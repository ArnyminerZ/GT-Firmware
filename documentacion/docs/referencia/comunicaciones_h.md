---
sidebar_position: 2
---

# Comunicaciones
Contiene todas las funciones relacionadas con el módulo RF.

## Funciones
### `rf_init`
**Descripción:**

Inicializa el módulo de RF.

**Parámetros:**
- `mod` ([`RFMod`](#rfmod)): La modulación que usar. `FSK` o `LORA`.
- `potencia` (`uint8_t`): La potencia a usar, va de `1` a `14`.
- `bitrate` (`uint16_t`): El bitrate de la comunicación (16 bits), revisar documentación (punto 2.5.4.8 de [RN2483-Commands.pdf](/docs/RN2483-Commands.pdf)).

### `rf_send`
**Descripción:**

Envia los datos por el puerto serie. Espera a una respuesta, y la guarda en `data`. El LED integrado estará encendido mientras se estén mandando los datos.

**Parámetros:**
- `dato` (`char[]`): El mensaje que mandar.

**Devuelve ([`RFError`](#rferror)):**

`OK` si no ha habido ningun error. O un error en caso contrario. Errores posibles: `TX_PARAM`, `TX_BUSY`, `TX_ERROR`

## Enumeraciones
### `RFError`
Notifica de posibles errores que han podido ocurrir durante la ejecución de una función.
- `OK`: No ha habido ningun error.
- `TX_PARAM`: Si ha habido algun error en el formato de los datos a mandar.
- `TX_BUSY`: Si se ha intentado transmitir cuando el módulo estaba ocupado.
- `TX_ERROR`: Ha habido algun error durante la transmisión (timeout).

### `RFMod`
Opciones de modulación RF, autodescriptivas.
- `LORA`
- `FSK`

## Configuraciones
### `COMM_SERIAL`
Se puede establecer a cualquier objeto de tipo [`Stream`](https://reference.arduino.cc/reference/en/language/functions/communication/stream/). Permite cambiar de puerto con facilidad. Por ejemplo, si se desea usar [`SoftwareSerial`](https://docs.arduino.cc/learn/built-in-libraries/software-serial), sería suficiente con establecer el valor a la variable que referencia la instance de la librería.

**Ejemplo:**
```cpp
#include <SoftwareSerial.h>

const byte rxPin = 2;
const byte txPin = 3;

// Set up a new SoftwareSerial object
SoftwareSerial mySerial (rxPin, txPin);

#define COMM_SERIAL mySerial
```
