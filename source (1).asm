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
liczba dd 16
.code
wyswietl_EAX PROC
pusha
mov ecx,0
cmp eax,-1
jg powyzej
neg eax
mov cl,1

powyzej:
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
cmp esi,0
je wyswietl ; skok, gdy ESI = 0
mov byte PTR znaki [esi], 20H ; kod spacji
dec esi ; zmniejszenie indeksu
jmp wypeln
wyswietl:
mov eax,0
mov ebx,1
znajdz_indeks:
mov al,znaki[ebx]
cmp al,20H
jne minusowanie
inc ebx
jmp znajdz_indeks

minusowanie:
cmp cl,0
je bez_minusa
mov byte PTR znaki [ebx-1], '-' ; kod nowego wiersza
bez_minusa:
mov byte PTR znaki [0], 0AH ; kod nowego wiersza
mov byte PTR znaki [12], 0AH ; kod nowego wiersza
; wyœwietlenie cyfr na ekranie
push dword PTR 12 ; liczba wyœwietlanych znaków
push dword PTR OFFSET znaki ; adres wyœw. obszaru
push dword PTR 1; numer urz¹dzenia (ekran ma numer 1)
call __write ; wyœwietlenie liczby na ekranie
add esp, 12
mov byte PTR znaki [ebx-1], 20H ; kod nowego wiersza
popa
ret
wyswietl_EAX ENDP

_main PROC

mov edx,0
mov ecx,1
mov eax,1
mov esi,31
call wyswietl_EAX
petla:
xor edx,1
jnz odejmowanie

add eax,ecx
jmp dalej

odejmowanie:
sub eax,ecx
jmp dalej


dalej:
call wyswietl_EAX
inc ecx
dec esi
jnz petla
push 0
call _ExitProcess@4
_main ENDP
END
