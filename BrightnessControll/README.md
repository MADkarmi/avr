układ dostosowujący (średnie) natężenie światła diody do natężenia światła panującego w otoczeniu (im ciemniej tym dioda powinna jaśniej świecić).
Do pomiaru natężenia padającego światła należy wykorzystać fotorezystor (LDR) i ADC. Należy zadbać o liniowość pomiaru od rezystancji fotorezystora, na jeden z dwóch sposobów:
*Mierzyć fotorezystor przez dzielnik napięcia (fotorezystor szeregowo z rezystorem o podobnej rezystancji) używając jako Aref napięcia zasilająego, obliczać w programie wartość rezystancji ze zmierzonej wartości napięcia (rozwiązanie programowe),
*Mierzyć fotorezystor przy użyciu źródła prądowego, używając jako Aref wbudowanego napięcia referencyjnego 1.1V (rozwiązanie sprzętowe).
*Rezystancja fotorezystora maleje liniowo ze wzrostem natężenia światła.