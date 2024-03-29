//
// Created by rhantolq on 26.10.2019.
//

#ifndef PROJ2_POSET_H
#define PROJ2_POSET_H

#ifdef __cplusplus
#include <cstddef>
#else
#include "stddef.h"
#include "stdbool.h"
#endif

#ifdef __cplusplus
namespace jnp1 {
    extern "C" {
#endif
        unsigned long poset_new(void);

        void poset_delete(unsigned long id);

        size_t poset_size(unsigned long id);

        bool poset_insert(unsigned long id, char const *value);

        bool poset_remove(unsigned long id, char const *value);

        bool poset_add(unsigned long id, char const *value1, char const *value2);

        bool poset_del(unsigned long id, char const *value1, char const *value2);

        bool poset_test(unsigned long id, char const *value1, char const *value2);

        void poset_clear(unsigned long id);
#ifdef __cplusplus
    }
}
#endif


#endif //PROJ2_POSET_H
