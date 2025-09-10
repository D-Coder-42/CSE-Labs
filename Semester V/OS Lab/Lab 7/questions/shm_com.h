#ifndef SHM_COM_H
#define SHM_COM_H

#define TEXT_SZ 2048

typedef struct {
    int written;
    char payload[TEXT_SZ];
} shared;

#endif
