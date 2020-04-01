//
// Created by uri on 6/7/19.
//

#ifndef REDIS_REDIS_ERR_H
#define REDIS_REDIS_ERR_H


int initErrorStrings ();
const char *redisError(unsigned int err);
void freeErrorStrings();

#endif //REDIS_REDIS_ERR_H
