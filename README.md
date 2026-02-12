# ch8

Emulador CHIP-8 escrito em C, com foco em estudo de emulação, organização de código e compreensão do funcionamento do sistema clássico CHIP-8. Trata-se de uma implementação funcional, com limitações de robustez e compatibilidade.

## Organização do projeto
```
.
├── include/        # Interfaces públicas (cabeçalhos)
├── src/
│   ├── core/       # Núcleo do emulador (CPU, memória, display, opcodes)
│   ├── ui/         # Interface baseada em ncurses
│   ├── cli.c       # Parsing de argumentos
│   ├── emulator.c  # Gerenciamento da execução
│   └── main.c      # Ponto de entrada
```

## Como compilar

Requer:
- Compilador C
- ncurses

```sh
make
```

Caso não utilize `make`, é possível compilar diretamente com:

```sh
cc -Iinclude src/*.c src/core/*.c src/ui/*.c -lncurses -o ch8
```

## Como executar

```sh
./ch8 [opções] <rom>
```

### Opções

```
-f <hz>    Define a frequência da CPU (padrão: 700 Hz)
-h         Exibe mensagem de ajuda
```

### Exemplo

```sh
./ch8 -f 800 roms/PONG.ch8
```

### Mapeamento de teclas

O teclado hexadecimal do CHIP-8 (4x4) é mapeado para teclas do teclado QWERTY da seguinte forma:

```
CHIP-8        Teclado

1 2 3 C       1 2 3 4
4 5 6 D   →   Q W E R
7 8 9 E       A S D F
A 0 B F       Z X C V
```

## Materiais de referência
- [Wikipedia - CHIP-8](https://en.wikipedia.org/wiki/CHIP-8)
- [Guide to making a CHIP-8 emulator](https://tobiasvl.github.io/blog/write-a-chip-8-emulator)
- [Cowgod's Chip-8 Technical Reference v1.0](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)
- [Github - trapext/chip-8_documentation](https://github.com/trapexit/chip-8_documentation)
- [Github - mattmikolay/chip-8](https://github.com/mattmikolay/chip-8)
- [Github - Timendus/chip8-test-suite](https://github.com/Timendus/chip8-test-suite)
