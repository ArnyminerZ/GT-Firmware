---
sidebar_position: 4
---

# Conexiones
El mapa de conexiones del módulo.

## Módulo RF
| Arduino | Módulo RF   |
|---------|-------------|
| `D2`    | `RESET`     |
| `TX`    | `RX`        |
| `RX`    | `TX`        |

> Notas:
> El pin RX tiene una resistencia pull-up, hay que puentearla.

## Sensores
| Arduino | Sensor                        |
|---------|-------------------------------|
| `A0`    | Potenciómetro del acelerador. |
| `A1`    | Potenciómetro del freno.      |

## Feedback
Se usa el led integrado en el Arduino (`LED_BUILTIN`), para indicar cuando se están mandando datos. Ojo porque este LED está invertido, de modo que `HIGH` es apagado, y `LOW` encendido.
