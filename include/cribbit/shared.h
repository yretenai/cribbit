//
// Created by Chronovore on 7/16/2023.
//

#ifndef CRIBBIT_SHARED_H
#define CRIBBIT_SHARED_H

#ifdef _WIN32
#define CRIBBIT_DECL __cdecl
#ifdef CRIBBIT_EXPORTING
#define CRIBBIT_SHARED __declspec(dllexport)
#else
#define CRIBBIT_SHARED __declspec(dllimport)
#endif
#else
#define CRIBBIT_DECL
#define CRIBBIT_SHARED
#endif

#endif //CRIBBIT_SHARED_H
