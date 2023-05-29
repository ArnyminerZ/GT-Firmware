# Firmware UPV GT
Este es el firmware para los módulos del proyecto GT de la UPV.

## Conexiones
### Módulo RF
| Arduino | Módulo RF   |
|---------|-------------|
| `D2`    | `RESET`     |
| `TX`    | `RX`        |
| `RX`    | `TX`        |

> Notas:
> El pin RX tiene una resistencia pull-up, hay que puentearla.

### Sensores
| Arduino | Sensor                        |
|---------|-------------------------------|
| `A0`    | Potenciómetro del acelerador. |
| `A1`    | Potenciómetro del freno.      |

### Feedback
Se usa el led integrado en el Arduino (`LED_BUILTIN`), para indicar cuando se están mandando datos. Ojo porque este LED está invertido, de modo que `HIGH` es apagado, y `LOW` encendido.

# Referencia
Ver [REFERENCIA.md](/REFERENCIA.md).

# Posibles problemas
1. Estamos usando el pin del monitor serie del Arduino para la comunicación con el módulo RF. Esto además de dar problemas a la hora de subir el código, es poco recomendable, ya que no permite depurar la ejecución, y detectar posibles problemas.
    Posibles soluciones:
    - Usar [Software Serial](https://docs.arduino.cc/learn/built-in-libraries/software-serial). Que aunque es bastante exigente en cuanto a memoria, podría ser una solución válida.
