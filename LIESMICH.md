# QtDoppelKopf

Copyright tristhaus 2022 und später.

![main](/../screenshot/screenshot.png?raw=true)

Ein Programm zum interaktiven Plotten einer Vektorfelddarstellung von Funktionen in der komplexen Ebene.

## Für Nutzer

Das Programm ist im Beta-Stadium.

| Name  | Code  |
|---|---|
| Konstante | `-0.357 + 3.1i`, `0.5I` |
| Unabhängige Variable | `z` |
| Einfache Arithmetik | `+ - * \` |
| Potenz | `^` |
| Gruppierung |  `()` |
| Funktionen | [`Re`](https://en.cppreference.com/w/cpp/numeric/complex/real), [`Im`](https://en.cppreference.com/w/cpp/numeric/complex/imag), [`abs`](https://en.cppreference.com/w/cpp/numeric/complex/abs), [`norm`](https://en.cppreference.com/w/cpp/numeric/complex/norm), [`conj`](https://en.cppreference.com/w/cpp/numeric/complex/conj), [`sin`](https://en.cppreference.com/w/cpp/numeric/complex/sin), [`cos`](https://en.cppreference.com/w/cpp/numeric/complex/cos), [`tan`](https://en.cppreference.com/w/cpp/numeric/complex/tan), [`sqrt`](https://en.cppreference.com/w/cpp/numeric/complex/sqrt), [`exp`](https://en.cppreference.com/w/cpp/numeric/exp), [`ln`](https://en.cppreference.com/w/cpp/numeric/complex/log) |

`Re` und `Im` ergeben beide einen Realteil. Es folgt, dass zur Rekonstrunktion von `z` der Aufruf von `Re(z) + Im(z) * i` nötig ist.

Siehe Release Abschnitt für aktuelle Informationen bezüglich unterstützter Features. Siehe Issues Abschnitt für geplante Verbesserungen. Ich nehme Kommentare zu dem Programm unter `qtpollynom` at `gmail.com` entgegen.

## Für Entwickler

Siehe [README.md](README.md).

## Lizenz

Alle Quelldateien lizensiert unter der GPL v3, siehe LICENSE für die Bedingungen, siehe auch [deutschsprachige Hinweise der FSF](https://www.gnu.org/licenses/quick-guide-gplv3).

## Anerkennungen

Graphische Nutzeroberfläche gebaut mit [Qt](https://doc.qt.io/).

Icon (Buchstaben und Pfeil) von tristhaus.

[QCustomPlot](https://www.qcustomplot.com/) Bibliothek (Version 2.1.0) von Emanuel Eichhammer genutzt unter der [GPL v3](https://www.gnu.org/licenses/gpl-3.0.html).

GoogleTest genutzt unter folgenden Bedingungen:

```
Copyright 2008, Google Inc.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

    * Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
copyright notice, this list of conditions and the following disclaimer
in the documentation and/or other materials provided with the
distribution.
    * Neither the name of Google Inc. nor the names of its
contributors may be used to endorse or promote products derived from
this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
```
