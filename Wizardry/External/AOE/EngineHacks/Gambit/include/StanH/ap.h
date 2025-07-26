#ifndef GBAFE_AP_H
#define GBAFE_AP_H

#include "common.h"
#include "proc.h"

// I used to call those "TCS" in my doc -Stan

typedef struct APHandle APHandle;
typedef struct APHandle AnimHandle;
typedef struct APHandle TCStruct;

extern struct APHandle gApPool[];

// struct APHandle* AP_Create(const void *definition, int depth); //! FE8U = (0x0800927C+1)
// void AP_Delete(struct APHandle* ap); //! FE8U = (0x080092A4+1)
// void AP_Update(struct APHandle* ap, u16 x, u16 y); //! FE8U = (0x080092BC+1)
void AP_SetAnimation(struct APHandle* ap, int index); //! FE8U = (0x08009518+1)
//void AP_SetDefinition(struct APHandle* ap, const void* definition); //! FE8U = (0x08009548+1)

// struct Proc* APProc_Create(const void* definition, u16 x, u16 y, u16 tileBase, int animIndex, int depth); //! FE8U = (0x08009718+1)
// void APProc_SetParameters(struct Proc* apProc, u16 x, u16 y, u16 tileBase); //! FE8U = (0x08009798+1)
// void APProc_Delete(struct Proc*); //! FE8U = (0x080097B4+1)
void APProc_DeleteAll(void); //! FE8U = (0x080097C0+1)
// int APProc_Exists(void); //! FE8U = (0x080097D1)

#endif // GBAFE_AP_H
