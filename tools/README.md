# Override tools

```bash
chmod +x tools/offset tools/address
```

## Offset (32-bit EIP)

```shell
Override$ ./tools/offset
```
```shell
Pattern length: 123

Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0

EIP address (hex): 0x37634136
Offset: 80 bytes
```

## Address (32- or 64-bit)

```bash
Override$ ./tools/address 
```
```bash
Hex address: 0x37634136
"\x37\x63\x41\x36"[::-1]
```