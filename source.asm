; wczytywanie i wyœwietlanie tekstu wielkimi literami
; (inne znaki siê nie zmieniaj¹)
.686
.model flat
extern _ExitProcess@4 : PROC
extern _MessageBoxW@16 : PROC
extern __write : PROC ; (dwa znaki podkreœlenia)
extern __read : PROC ; (dwa znaki podkreœlenia)
public _main
.data

dziesiec dd 10 ; mno¿nik
znaki db 12 dup (?)
dekoder db '0123456789ABCDEF'
liczba dd 17
.code
wyswietl_EAX PROC
pusha
cmp ebx,0
je koniec
mov eax,[ebx]
mov esi, 10 ; indeks w tablicy 'znaki'
mov ebx, 10 ; dzielnik równy 10
konwersja:
mov edx, 0 ; zerowanie starszej czêœci dzielnej
div ebx ; dzielenie przez 10, reszta w EDX,
; iloraz w EAX
add dl, 30H ; zamiana reszty z dzielenia na kod
; ASCII
mov znaki [esi], dl; zapisanie cyfry w kodzie ASCII
dec esi ; zmniejszenie indeksu
cmp eax, 0 ; sprawdzenie czy iloraz = 0
jne konwersja ; skok, gdy iloraz niezerowy
; wype³nienie pozosta³ych bajtów spacjami i wpisanie
; znaków nowego wiersza
wypeln:
or esi, esi
jz wyswietl ; skok, gdy ESI = 0
mov byte PTR znaki [esi], 20H ; kod spacji
dec esi ; zmniejszenie indeksu
jmp wypeln
wyswietl:
mov byte PTR znaki [0], 0AH ; kod nowego wiersza
mov byte PTR znaki [11], 0AH ; kod nowego wiersza
; wyœwietlenie cyfr na ekranie
push dword PTR 12 ; liczba wyœwietlanych znaków
push dword PTR OFFSET znaki ; adres wyœw. obszaru
push dword PTR 1; numer urz¹dzenia (ekran ma numer 1)
call __write ; wyœwietlenie liczby na ekranie
add esp, 12 ; usuniêcie parametrów ze stosu
koniec:
popa
ret
wyswietl_EAX ENDP

_main PROC
mov ebx,OFFSET liczba
call wyswietl_EAX

push 0
call _ExitProcess@4
_main ENDP
END
