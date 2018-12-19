//
// Created by pgl on 2018/12/17.
//

#ifndef DISCRETEEARTH_EXPORTS_H
#define DISCRETEEARTH_EXPORTS_H

#ifndef GT_API
    #ifdef _MSC_VER
        # if GT_CREATE_SHARED_LIBRARY
            #  define GT_API __declspec(dllexport)
        # elif GT_LINKAS_SHARED_LIBRARY
            #  define GT_API __declspec(dllimport)
        # endif
    #elif __GNUC__ >= 4 || defined(__clang__)
        # define GT_API __attribute__((visibility ("default")))
    #endif  // _MSC_VER
#endif  // GT_API

#ifndef GT_API
    # define GT_API
#endif  // GT_API
#endif //DISCRETEEARTH_EXPORTS_H
