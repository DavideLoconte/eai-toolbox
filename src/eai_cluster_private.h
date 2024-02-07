#ifndef EAI_TOOLBOX_EAI_CLUSTER_PRIVATE_H
#define EAI_TOOLBOX_EAI_CLUSTER_PRIVATE_H

#include "eai_cluster.h"

/**
 * Compute the euclidean distance between two points
 * @param a the first point
 * @param b the second point
 * @return the euclidean distance between two points
 */
ulib_float eai_distance(EaiNArray(ulib_float)* a, EaiNArray(ulib_float)* b, ulib_uint shape);

/**
 * Compute the squared euclidean distance between two points
 * @param a the first point
 * @param b the second point
 * @return the euclidean distance between two points
 */
ulib_float eai_squared_distance(EaiNArray(ulib_float)* a, EaiNArray(ulib_float)* b, ulib_uint shape);

#endif
