#ifndef UIP_CONF_H
#define UIP_CONF_H

#include <stdint.h>

// 1. Mappatura dei tipi di dato (Per GCC e x86_64)
typedef uint8_t  u8_t;
typedef uint16_t u16_t;
typedef uint32_t u32_t;

// Tipo per le statistiche interne di uIP
typedef unsigned short uip_stats_t;

// 2. Limiti di Memoria (I parametri che ti davano "undeclared")
// Definisci la dimensione massima del pacchetto Ethernet (1500 byte è lo standard)
#define UIP_CONF_BUFFER_SIZE     1500

// Quante connessioni TCP simultanee vuoi gestire? (A noi ne basta 1 o 2)
#define UIP_CONF_MAX_CONNECTIONS 4

// Quante porte in ascolto simultanee?
#define UIP_CONF_MAX_LISTENPORTS 4

// 3. Impostazioni di Sistema per x86
// I processori Intel/AMD usano il Little Endian
#define UIP_CONF_BYTE_ORDER      1234

// Disabilitiamo funzionalità extra che per ora non ci servono
#define UIP_CONF_LOGGING         0
#define UIP_CONF_UDP             0
#define UIP_CONF_STATISTICS      0

#endif /* UIP_CONF_H */
