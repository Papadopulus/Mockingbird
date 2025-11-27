# Mockingbird - Detaljna Dokumentacija

## Sadržaj
1. [Uvod u Mokovanje (Mocking)](#uvod-u-mokovanje-mocking)
2. [Mokovanje u C++](#mokovanje-u-c)
3. [CMocka Framework](#cmocka-framework)
4. [Arhitektura Mockingbird Aplikacije](#arhitektura-mockingbird-aplikacije)
5. [Detaljna Analiza Komponenti](#detaljna-analiza-komponenti)
6. [Primeri Korišćenja](#primeri-korišćenja)

---

## Uvod u Mokovanje (Mocking)

### Šta je Mokovanje?

**Mokovanje** (eng. Mocking) je tehnika u testiranju softvera gde se prave **lažne implementacije** funkcija ili objekata umesto pravih. Ovo omogućava:

- **Izolaciju koda koji se testira** - testirate samo jednu funkciju bez zavisnosti
- **Kontrolu ponašanja** - možete definisati šta će "lažna" funkcija vratiti
- **Verifikaciju poziva** - možete proveriti da li je funkcija pozvana sa očekivanim parametrima
- **Simulaciju grešaka** - možete testirati kako vaš kod reaguje na greške

### Zašto je Mokovanje Važno?

Zamislite da imate funkciju koja čita podatke iz baze podataka:

```c
// Originalna funkcija
int get_user_age(int user_id) {
    // Povezuje se na bazu podataka
    // Izvršava SQL upit
    // Vraća starost korisnika
}
```

Bez mokovanja, svaki test bi:
- Morao da se poveže na pravu bazu
- Bio spor
- Mogao da padne ako baza nije dostupna
- Mogao da promeni podatke u bazi

Sa mokovanjem, možete:
- Simulirati odgovor baze bez prave konekcije
- Testirati brzo i pouzdano
- Testirati granične slučajeve (npr. šta ako korisnik ne postoji?)

---

## Mokovanje u C++

### Osnovni Koncepti

U C jeziku (koji Mockingbird podržava), mokovanje se radi tako što:

1. **Napravite funkciju sa istim potpisom** kao originalna
2. **Kontrolišete povratnu vrednost** umesto da izvršavate pravu logiku
3. **Proveravate ulazne parametre** da biste bili sigurni da je funkcija pozvana ispravno

### Primer Ručnog Mokovanja

Originalna funkcija:
```c
// hardware.h
int read_sensor(int sensor_id);
```

Originalna implementacija:
```c
// hardware.c
int read_sensor(int sensor_id) {
    // Čita pravu vrednost sa hardverskog senzora
    return actual_hardware_read(sensor_id);
}
```

Mokirana verzija za testiranje:
```c
// hardware_mock.c
int read_sensor(int sensor_id) {
    // Umesto da čita pravu vrednost, vraća unapred definisanu
    return 25;  // Simulirana temperatura
}
```

### Problem sa Ručnim Mokovanjem

Ručno pisanje mock funkcija je:
- **Zamorno** - za svaku funkciju morate napisati mock verziju
- **Podložno greškama** - lako je napraviti grešku u potpisu
- **Teško za održavanje** - ako se originalna funkcija promeni, morate ažurirati mock

**Mockingbird rešava ovaj problem automatskim generisanjem mock funkcija!**

---

## CMocka Framework

### Šta je CMocka?

**CMocka** je C testing framework koji pruža:
- Funkcije za verifikaciju parametara (`check_expected`, `check_expected_ptr`)
- Funkcije za vraćanje vrednosti (`mock()`, `mock_type_ptr()`)
- Makroe za očekivane vrednosti (`expect_value`, `will_return`)

### CMocka API - Objašnjenje

#### 1. `check_expected(param)` / `check_expected_ptr(param)`
Verifikuje da je parametar koji je prosleđen funkciji jednak očekivanoj vrednosti.

```c
void my_mock_function(int value) {
    check_expected(value);  // Proverava da li je 'value' očekivan
}

// U testu:
expect_value(my_mock_function, value, 42);  // Očekujemo da 'value' bude 42
my_mock_function(42);  // Test prolazi
my_mock_function(10);  // Test pada!
```

#### 2. `mock()` / `mock_type_ptr(type)`
Vraća unapred definisanu vrednost iz mock funkcije.

```c
int get_data(void) {
    return (int)mock();  // Vraća vrednost definisanu sa will_return
}

// U testu:
will_return(get_data, 100);  // Kada se pozove get_data, vratiće 100
int result = get_data();  // result = 100
```

#### 3. `expect_value(func, param, value)`
Definiše očekivanu vrednost parametra.

```c
expect_value(read_sensor, sensor_id, 5);  // Očekujemo sensor_id = 5
```

#### 4. `will_return(func, value)`
Definiše šta će mock funkcija vratiti.

```c
will_return(read_sensor, 25);  // read_sensor će vratiti 25
```

### Kompletan CMocka Primer

```c
// ===== mock_sensor.c =====
#include <cmocka.h>

// Mock funkcija
int read_sensor(int sensor_id) {
    check_expected(sensor_id);      // Verifikuj parametar
    return (int)mock();             // Vrati predefinisanu vrednost
}

// ===== test_temperature.c =====
#include <cmocka.h>

// Funkcija koju testiramo
int get_average_temperature(void) {
    int sum = 0;
    for (int i = 0; i < 3; i++) {
        sum += read_sensor(i);
    }
    return sum / 3;
}

// Test
static void test_average_temperature(void **state) {
    // Očekujemo tri poziva sa sensor_id = 0, 1, 2
    expect_value(read_sensor, sensor_id, 0);
    will_return(read_sensor, 20);
    
    expect_value(read_sensor, sensor_id, 1);
    will_return(read_sensor, 25);
    
    expect_value(read_sensor, sensor_id, 2);
    will_return(read_sensor, 30);
    
    // Pozivamo funkciju koju testiramo
    int avg = get_average_temperature();
    
    // Proveravamo rezultat
    assert_int_equal(avg, 25);  // (20 + 25 + 30) / 3 = 25
}
```

---

## Arhitektura Mockingbird Aplikacije

### Pregled Sistema

```
┌─────────────────────────────────────────────────────────────────┐
│                    MOCKINGBIRD APLIKACIJA                        │
├─────────────────────────────────────────────────────────────────┤
│                                                                  │
│  ┌───────────────┐    ┌──────────────┐    ┌─────────────────┐   │
│  │   MainForm    │───▶│   FileForm   │    │   FilesForm     │   │
│  │  (Navigacija) │    │(Jedan fajl)  │    │(Više fajlova)   │   │
│  └───────────────┘    └──────────────┘    └─────────────────┘   │
│          │                   │                    │              │
│          │                   ▼                    ▼              │
│          │           ┌──────────────────────────────────┐        │
│          │           │         MockingFiles             │        │
│          │           │    (Generisanje Mock Fajlova)    │        │
│          │           └──────────────────────────────────┘        │
│          │                         │                             │
│          │                         ▼                             │
│          │           ┌──────────────────────────────────┐        │
│          │           │           ctags.exe              │        │
│          │           │   (Parsiranje C Funkcija)        │        │
│          │           └──────────────────────────────────┘        │
│          │                         │                             │
│          ▼                         ▼                             │
│  ┌───────────────┐    ┌──────────────────────────────────┐       │
│  │ CommonFunctions│    │     Generisani Mock Fajlovi     │       │
│  │ (Pomoćne funk.)│    │  (.c fajlovi sa CMocka kodom)   │       │
│  └───────────────┘    └──────────────────────────────────┘       │
│                                                                  │
└─────────────────────────────────────────────────────────────────┘
```

### Glavne Klase

| Klasa | Odgovornost |
|-------|-------------|
| `MainForm` | Glavni prozor aplikacije, navigacija između formi |
| `FileForm` | Mokovanje jednog C fajla |
| `FilesForm` | Mokovanje svih zavisnosti projekta |
| `MockingFiles` | Glavna logika za generisanje mock funkcija |
| `CommonFunctions` | Pomoćne funkcije (konverzija stringova, čuvanje fajlova) |
| `IncludeModuleSettings` | Konfiguracija za napredna podešavanja mock-a |

---

## Detaljna Analiza Komponenti

### 1. MockingFiles Klasa

Ovo je **srce aplikacije**. Ova klasa:
1. Poziva `ctags.exe` za parsiranje C fajlova
2. Analizira izlaz ctags-a da bi izvukao funkcije
3. Generiše CMocka mock funkcije

#### Metoda: `ProcessFile`

```cpp
std::vector<std::string> MockingFiles::ProcessFile(
    const std::string& a_filePath, 
    IncludeModuleSettings^ moduleSettings)
```

**Šta radi:**
1. Gradi komandu za ctags:
   ```
   ctags.exe --c-kinds=+px --fields=+iaS --languages=C -o tags_file source.c
   ```
2. Pokreće ctags kao proces
3. Parsira generisani tags fajl
4. Za svaku funkciju generiše mock verziju

**Primer ulaza i izlaza:**

Ulazni fajl `sensor.c`:
```c
int read_temperature(int sensor_id) {
    return hardware_read(sensor_id);
}

void set_threshold(int value, float* buffer) {
    *buffer = value * 1.5;
}
```

Generisani mock fajl:
```c
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <string.h>
#include "cmocka.h"
#include "sensor.h"

// Mocked functions
int read_temperature(int sensor_id)
{
    check_expected(sensor_id);
    return (int)mock();
}

#define MOCK_read_temperature(a_sensor_id, a_ret)\
    expect_value(read_temperature, sensor_id, a_sensor_id);\
    will_return(read_temperature, a_ret)\

void set_threshold(int value, float* buffer)
{
    check_expected(value);
    check_expected_ptr(buffer);
}

#define MOCK_set_threshold(a_value, a_buffer)\
    expect_value(set_threshold, value, a_value);\
    expect_value(set_threshold, buffer, a_buffer)\

```

#### Metoda: `MockNonStaticFunctions`

```cpp
std::vector<std::string> MockingFiles::MockNonStaticFunctions(
    const std::vector<std::string>& a_functions)
```

**Korak po korak objašnjenje:**

1. **Parsira svaku funkciju:**
   ```cpp
   // Input: "int\tread_sensor(int sensor_id"
   // Output: returnType = "int", functionName = "read_sensor", parameters = "int sensor_id"
   ```

2. **Generiše potpis mock funkcije:**
   ```cpp
   mockFunction << returnType << " " << functionName << "(";
   // Dodaje parametre...
   mockFunction << ")\n{\n";
   ```

3. **Za svaki parametar dodaje verifikaciju:**
   ```cpp
   if (param.find('*') != std::string::npos) {
       // Pointer parametar
       mockFunction << "\tcheck_expected_ptr(" << paramName << ");\n";
   } else {
       // Običan parametar
       mockFunction << "\tcheck_expected(" << paramName << ");\n";
   }
   ```

4. **Dodaje povratnu vrednost:**
   ```cpp
   if (returnType != "void") {
       if (returnType.find('*') != std::string::npos) {
           // Pointer povratni tip
           mockFunction << "\treturn mock_type_ptr(" << returnType << ");\n";
       } else {
           mockFunction << "\treturn (" << returnType << ")mock();\n";
       }
   }
   ```

5. **Generiše makro za lakšu upotrebu:**
   ```cpp
   #define MOCK_funkcija(a_param1, a_param2, a_ret)\
       expect_value(funkcija, param1, a_param1);\
       expect_value(funkcija, param2, a_param2);\
       will_return(funkcija, a_ret)\
   ```

### 2. Parsiranje sa ctags

#### Šta je ctags?

`ctags` je alat koji generiše "tags" fajl - indeks svih simbola (funkcija, promenljivih, tipova) u izvornom kodu.

#### Primer ctags izlaza

Za fajl:
```c
int calculate_sum(int a, int b) {
    return a + b;
}

static void helper_function(void) {
    // ...
}
```

ctags generiše:
```
calculate_sum   source.c   /^int calculate_sum(int a, int b)$/;    f   signature:(int a, int b)
helper_function source.c   /^static void helper_function(void)$/;  f   signature:(void)
```

#### Kako Mockingbird parsira ovaj izlaz

```cpp
std::vector<std::string> MockingFiles::ParseTagsFile(const std::string& a_tagsFilePath)
{
    // 1. Čita fajl liniju po liniju
    while (std::getline(file, line)) {
        // 2. Preskače komentare (linije koje počinju sa !)
        if (line[0] == '!')
            continue;
        
        // 3. Razdvaja delove linije po tab karakteru
        std::regex re("\t(?![^\(]*\;)");
        std::sregex_token_iterator iter(line.begin(), line.end(), re, -1);
        
        // 4. Izvlači informacije
        std::string functionName = parts[0];     // Ime funkcije
        std::string fileName = parts[1];         // Ime fajla
        std::string exCommand = parts[2];        // Potpis funkcije
        std::string extra = parts[3];            // Tip (f = funkcija)
        std::string functionParameters = parts[4]; // Parametri
        
        // 5. Preskače static funkcije
        if (exCommand.find("static") != std::string::npos)
            continue;
        
        // 6. Izvlači povratni tip i gradi string za mock
        functions.push_back(returnType + "\t" + functionName + "(" + parameters);
    }
}
```

### 3. Rukovanje Static Funkcijama

Mockingbird ima posebnu funkcionalnost za static funkcije:

#### Problem
Static funkcije su vidljive samo unutar fajla gde su definisane. Da biste ih testirali, morate:
1. Ukloniti `static` ključnu reč
2. Ili koristiti makro trik

#### Mockingbird Rešenje

Aplikacija može da ubaci sledeće u izvorni kod:

```c
#ifdef UNIT_TESTING
#define STATIC
#define STATIC_INLINE
#else
#define STATIC static
#define STATIC_INLINE static inline
#endif
```

Zatim zamenjuje `static` sa `STATIC`:

```c
// Pre obrade
static int helper(int x) { return x * 2; }

// Posle obrade
STATIC int helper(int x) { return x * 2; }
```

Kada kompajlirate sa `-DUNIT_TESTING`, `STATIC` postaje prazno, pa je funkcija vidljiva za testove!

### 4. CommonFunctions Klasa

#### Konverzija Stringova

```cpp
std::string CommonFunctions::toStandardString(System::String^ string)
{
    // Konvertuje .NET String^ u std::string
    using System::Runtime::InteropServices::Marshal;
    System::IntPtr pointer = Marshal::StringToHGlobalAnsi(string);
    char* charPointer = reinterpret_cast<char*>(pointer.ToPointer());
    std::string returnString(charPointer, string->Length);
    Marshal::FreeHGlobal(pointer);
    return returnString;
}
```

Ovo je potrebno jer aplikacija koristi C++/CLI koji kombinuje .NET i native C++.

#### Čuvanje Mock Fajla

```cpp
void CommonFunctions::SaveMockedFile(
    const std::string& mockedFilePath,
    const std::string& a_originalFilePath,
    const std::vector<std::string>& a_mockFunctions)
{
    std::ofstream writer(mockedFilePath);
    
    // 1. Dodaje CMocka header-e
    writer << "#include <stdarg.h>\n";
    writer << "#include <stddef.h>\n";
    writer << "#include <setjmp.h>\n";
    writer << "#include <string.h>\n";
    writer << "#include \"cmocka.h\"\n";
    
    // 2. Pronalazi i dodaje povezane .h fajlove
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (entry.path().extension() == ".h") {
            writer << "#include \"" << entry.path().filename().string() << "\"\n";
        }
    }
    
    // 3. Piše mock funkcije
    writer << "\n// Mocked functions\n";
    for (const auto& mockFunction : a_mockFunctions) {
        writer << mockFunction;
    }
}
```

### 5. Napredna Podešavanja (IncludeModuleSettings)

Za složenije scenarije, korisnik može konfigurisati kako se generišu mock funkcije:

```cpp
ref struct ParameterSettings
{
    System::String^ paramName;   // Ime parametra
    System::String^ setting;     // "None", "Ptr", ili "Length"
};

ref struct FunctionSettings
{
    System::String^ functionName;
    System::String^ returnType;
    System::Collections::Generic::List<ParameterSettings^>^ parameters;
};
```

Ovo omogućava posebno rukovanje za funkcije koje kopiraju podatke:

```cpp
// Originalna funkcija
int read_data(uint8_t* buffer, size_t length);

// Sa podešavanjima Ptr=buffer, Length=length, generiše:
int read_data(uint8_t* buffer, size_t length)
{
    uint8_t* cpy_ptr = mock_ptr_type(uint8_t*);
    size_t cpy_length = (size_t)mock();
    if(cpy_ptr != NULL && cpy_length > 0U)
    {
        memcpy(buffer, cpy_ptr, cpy_length);
    }
    return (int)mock();
}
```

Ovo omogućava testiranje funkcija koje vraćaju podatke kroz pointer parametre!

---

## Primeri Korišćenja

### Primer 1: Mokovanje Jednog Fajla

1. Pokrenite Mockingbird
2. Kliknite na "Mock One File"
3. Izaberite C fajl (npr. `network.c`)
4. Izaberite lokaciju za čuvanje
5. Dobijate `network.c` sa mock funkcijama

### Primer 2: Mokovanje Svih Zavisnosti

1. Kliknite na "Mock Files"
2. Izaberite folder sa vašim MUT (Module Under Test)
3. Aplikacija pronalazi sve `#include` direktive
4. Označite koje zavisnosti želite da mokujete
5. Izaberite source folder gde se nalaze originalni fajlovi
6. Aplikacija generiše mocks folder sa svim mock fajlovima

### Primer 3: Testiranje sa Generisanim Mockovima

```c
// ===== test_my_module.c =====
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

// Uključite vaš modul
#include "my_module.h"

// Funkcija iz my_module.c koju testiramo
extern int process_sensor_data(void);

// Test
static void test_process_sensor_data(void **state) {
    // Koristite generisani makro
    MOCK_read_sensor(0, 100);  // sensor_id=0, vraća 100
    MOCK_read_sensor(1, 200);  // sensor_id=1, vraća 200
    
    int result = process_sensor_data();
    
    assert_int_equal(result, 150);  // Očekujemo prosek
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_process_sensor_data),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
```

---

## Zaključak

Mockingbird automatizuje proces koji bi inače bio ručan, zamoran i podložan greškama:

1. **Parsira** C izvorni kod koristeći ctags
2. **Analizira** potpise funkcija
3. **Generiše** CMocka-kompatibilne mock funkcije
4. **Kreira** makroe za lakšu upotrebu u testovima

Ova automatizacija značajno ubrzava razvoj unit testova za C projekte i smanjuje mogućnost ljudske greške pri ručnom pisanju mock funkcija.
