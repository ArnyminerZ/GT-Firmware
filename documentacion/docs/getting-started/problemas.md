---
sidebar_position: 5
---

# Posibles problemas
Anotamos posibles problemas que puedan ocurrir con las implementaciones actuales.

1. Estamos usando el pin del monitor serie del Arduino para la comunicación con el módulo RF. Esto además de dar problemas a la hora de subir el código, es poco recomendable, ya que no permite depurar la ejecución, y detectar posibles problemas.
    Posibles soluciones:
    - Usar [Software Serial](https://docs.arduino.cc/learn/built-in-libraries/software-serial). Que aunque es bastante exigente en cuanto a memoria, podría ser una solución válida.
