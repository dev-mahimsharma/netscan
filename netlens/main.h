#ifndef MAIN_H
#define MAIN_H


#ifdef __cplusplus
extern "C" {
#endif 

#define commands T;

typedef struct T T;

 struct T {

    char program_name[16];
    char from[16];
    char to[16];
    char feature[7];

};




#ifdef __CPLUSPLUS
}
#endif
#endif // !GET_IP_H