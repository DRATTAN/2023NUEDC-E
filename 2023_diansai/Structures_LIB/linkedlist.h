/*
 * linkedlist.h
 *
 *  Created on: 2023/07/16
 *      Author: RATTAN
 */

#ifndef STRUCTURES_LIB_LINKEDLIST_H_
#define STRUCTURES_LIB_LINKEDLIST_H_

#ifdef __cplusplus
extern "C" {
#endif

#define AccessLinkListFloat(L, index)      *(float*)DS_AccessLinkList(L, index)
#define AccessLinkListUint8(L, index)      *(uint8_t*)DS_AccessLinkList(L, index)
#define AccessLinkListUint16(L, index)     *(uint16_t*)DS_AccessLinkList(L, index)
#define AccessLinkListUint32(L, index)     *(uint32_t*)DS_AccessLinkList(L, index)
#define AccessLinkListInt16(L, index)      *(int16_t*)DS_AccessLinkList(L, index)
#define AccessLinkListInt8(L, index)       *(int8_t*)DS_AccessLinkList(L, index)
#define AccessLinkListInt32(L, index)      *(int32_t*)DS_AccessLinkList(L, index)

#include "stdlib.h"
#include "stdint.h"

struct _LinkListBody
{
    uint8_t pool[4];
    struct _LinkListBody * nextbodypoint;
};

struct _LinkListHeader
{
    uint32_t length;
    struct _LinkListBody *bodyPoint;

};

typedef struct _LinkListBody LinkListBody;
typedef struct _LinkListHeader LinkListHeader;
typedef LinkListHeader* LinkList;
typedef LinkListBody* LinkListMember;

static LinkListHeader *DS_CreateLinkListHeader(void);
static LinkListBody *DS_CreateLinkListBody(void);
static LinkListBody *DS_GetLinkListBodyAddr(LinkList, uint32_t);
static void DS_FreeLinkListHeader(LinkList);
static void DS_FreeLinkListBody(LinkListBody *);
static void DS_FreeLinkListBodyAll(LinkListBody *);
void DS_FreeLinkList(LinkList);
LinkListHeader *DS_CreateLinkList(uint32_t);
uint8_t DS_AppendLinkList(LinkList);
void *DS_AccessLinkList(LinkList, uint32_t);
uint32_t DS_GetLinkListLength(LinkList);
void DS_ClearLinkList(LinkList);

#ifdef __cplusplus
}
#endif

#endif /* STRUCTURES_LIB_LINKEDLIST_H_ */
