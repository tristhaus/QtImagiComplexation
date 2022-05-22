# QtImagiComplexation

Copyright tristhaus 2022 and later.

![main](/../screenshot/screenshot.png?raw=true)

A program to interactively plot a vector field representation of functions in the complex domain.

## For Users

This is beta status.

| Name  | Code  |
|---|---|
| Constant | `-0.357 + 3.1i`, `0.5I` |
| Independent variable | `z` |
| Basic arithmetic | `+ - * \` |
| Power | `^` |
| Grouping |  `()` |
| Functions | [`Re`](https://en.cppreference.com/w/cpp/numeric/complex/real), [`Im`](https://en.cppreference.com/w/cpp/numeric/complex/imag), [`abs`](https://en.cppreference.com/w/cpp/numeric/complex/abs), [`norm`](https://en.cppreference.com/w/cpp/numeric/complex/norm), [`conj`](https://en.cppreference.com/w/cpp/numeric/complex/conj), [`sin`](https://en.cppreference.com/w/cpp/numeric/complex/sin), [`cos`](https://en.cppreference.com/w/cpp/numeric/complex/cos), [`tan`](https://en.cppreference.com/w/cpp/numeric/complex/tan), [`sqrt`](https://en.cppreference.com/w/cpp/numeric/complex/sqrt), [`exp`](https://en.cppreference.com/w/cpp/numeric/complex/exp), [`ln`](https://en.cppreference.com/w/cpp/numeric/complex/log) |

Note that `Re` and `Im` both return as a real part. That is, to reconstruct `z`, call `Re(z) + Im(z) * i`.

See Release tab for up-to-date information on what is supported. See Issues tab for planned improvements. You may contact me at `qtpollynom` at `gmail.com` for comments on the program.

## For Developers

I am currently not looking for contributors since this is primarily my hobby.

Load the [QtImagiComplexationSuper.pro](QtImagiComplexationSuper.pro) into QtCreator >= 4.14.2, and configure the project. At least [QtImagiComplexation.pro](QtImagiComplexation/QtImagiComplexation.pro) should compile under Windows and Linux.

In order to compile the tests, you need to define one of two preprocessor constants
 * `_SKIP_LONG_TEST` if you wish to skip the long-running tests
 * `_USE_LONG_TEST` if you wish to execute those tests

`clang-tidy` has been added, be sure to configure QtCreator to use the [.clang-tidy](.clang-tidy) file.

## License

All source code licensed under GPL v3 (see LICENSE for terms).

## Attributions

Graphical user interface built using [Qt](https://doc.qt.io/).

Icon (letters and arrow) by tristhaus.

[QCustomPlot](https://www.qcustomplot.com/) library (Version 2.1.0) by Emanuel Eichhammer used under the [GPL v3](https://www.gnu.org/licenses/gpl-3.0.html).

GoogleTest used under the following conditions:

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
