//
// Created by pgl on 2018/12/17.
//

#ifndef DISCRETEEARTH_EXPORTS_H
#define DISCRETEEARTH_EXPORTS_H

#ifdef DISCRETEEARTH_EXPORTS
    #define DE_API __declspec(dllexport)
#else
    #define DE_API __declspec(dllimport)
#endif

#endif //DISCRETEEARTH_EXPORTS_H
