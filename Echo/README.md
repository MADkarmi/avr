Napisz program echo używający UART: odebrane znaki powinny być wysyłane na wyjście w kolejności odebrania. Nie wykorzystuj standardowego wejścia/wyjścia C, używaj bezpośrednio rejestru UDR0. Zapewnij maksymalną energooszczędność rozwiązania – należy wykorzystać przerwania UART, uśpić mikrokontroler w głównej pętli programu oraz wyłączyć zbędne podzespoły (timery, SPI, TWI, ADC).