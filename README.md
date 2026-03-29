# Prosta Geometria Analityczna (C + x86-64 ASM)

Projekt demonstruje integrację **C z x86-64 Assembly** w praktycznym zastosowaniu geometrii analitycznej.  
Aplikacja umożliwia wykonywanie podstawowych operacji geometrycznych oraz obliczeń funkcji kwadratowych.

---

## Funkcjonalności

- Obliczanie **długości odcinka** pomiędzy dwoma punktami  
- Wyznaczanie **punktu środkowego** odcinka  
- Wyznaczanie **równania prostej** przechodzącej przez dwa punkty  
- Wyznaczanie **symetralnej odcinka**  
- Obliczanie **obwodu i pola trójkąta**  
- Obliczanie **równania okręgu** o środku w punkcie S  
- Obsługa funkcji kwadratowej i wyznaczanie miejsc zerowych oraz wierzchołka paraboli  
- **Eksport wyników** do pliku tekstowego I zrzutu ekranu w formacie .png 

---

## Technologie

- **C** – logika programu i obsługa IO 
- **x86-64 Assembly (GNU)** – niskopoziomowe obliczenia matematyczne 
- **Makefile** – kompilacja i linkowanie modułów C + ASM 
- **Systemy operacyjne:** Linux / macOS (x86-64) 

## Instrukcja użycia  

### Kompilacja 

```bash make 

Uruchomienie ./main "NAZWA_PLIKU_WYJŚCIOWEGO.txt" "A = (X; Y)" "B = (X; Y)" 

Przykład: ./main "wynik.txt" "A = (-3;0)" "B = (-1;3)"  

Program wyliczy:  

- punkt C tak, aby ABC było trójkątem prostokątnym
- środek odcinka S
- równania prostych i symetralnej
- obwód i pole trójkąta
- równanie okręgu o środku w S
- analizę funkcji kwadratowej 
