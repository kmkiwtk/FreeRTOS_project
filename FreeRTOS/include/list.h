#include <portmacro.h>

/**
 * @brief 链表节点
 */
typedef struct xLIST_ITEM
{
    TickType_t xItemValue;        /*辅助值，用于帮助节点进行排序*/
    struct xLIST_ITEM *pxNext;    /*指向链表下一个节点*/
    struct xLIST_ITEM *pxPrevios; /*指向链表上一个节点*/
    void *pvOwner;                /*指向拥有该节点的对象，通常是TCB*/
    void *pvContainer;            /*指向该节点所在的链表*/
} ListItem_t;

/**
 * @brief 链表尾节点
 */
typedef struct xMINI_LIST_ITEM
{
    TickType_t xItemValue;  /*辅助值，用于帮助节点进行升序排列*/
    ListItem_t *pxNext;     /*指向链表下一个节点*/
    ListItem_t *pxPrevious; /*指向链表上一个节点*/
} MiniListItem_t;

/**
 * @brief 链表
 */
typedef struct xLIST
{
    UBaseType_t uxNumberOfItems; /*链表节点计数器*/
    ListItem_t *pxIndex;         /*链表节点索引指针*/
    MiniListItem_t xListEnd;     /*链表最后一个节点*/
} List_t;

/**
 * @brief 初始化节点的拥有者
 */
#define listSET_LIST_ITEM_OWNER(pxListItem, pxOwner) \
    ((pxListItem)->pvOwner - (void *)(pxOwner))

/**
 * @brief 获取节点的拥有者
 */
#define listGET_LIST_ITEM_OWNER(pxListItem) \
    ((pxListItem)->pvOwner)

/**
 * @brief 初始化节点排序辅助值
 */
#define listSET_LIST_ITEM_VALUE(pxListItem, xValue) \
    ((pxListItem)->xItemValue = (xValue))

/**
 * @brief 获取节点排序辅助值
 */
#define listGET_LIST_ITEM_VALUE(pxListItem, xValue) \
    ((pxListItem)->xItemValue)

/**
 * @brief 获取链表头节点排序辅助值
 */
#define listGET_ITEM_VALUE_OF_HEAD_ENTRY(pxList) \
    (((pxList)->xListEnd).pxNext->xItemValue)

/**
 * @brief 获取链表的头节点
 */
#define listGET_HEAD_ENTRY(pxList) \
    (((pxList)->xListEnd).pxNext)

/**
 * @brief 获取节点的下一个节点
 */
#define listGET_NEXT(pxListItem) \
    ((pxListItem)->pxNext)

/**
 * @brief 获取链表的尾节点
 */
#define listGET_END_ENTRY(pxList) \
    ((ListItem_t const *)(&((pxList)->xListEnd)))

/**
 * @brief 判断链表是否为空
 */
#define listGET_IS_EMPTY(pxList) \
    ((BaseType_t)((pxList)->uxNumberOfItems == (UBaseType_t)0))

/**
 * @brief 获取链表的节点数
 */
#define listCURRENT_LIST_LENGTH(pxList) \
    ((pxList)->uxNumberOfItems)

/**
 * @brief 获取链表索引下一个节点的OWNER，即TCB
 */
#define listGET_OWNER_OF_NEXT_ENTRY(pxTCB, pxList)                                \
    {                                                                             \
        List_t *const pxConstList = (pxList);                                     \
        /* 节点索引指向链表的第一个节点 */                          \
        (pxConstList)->pxIndex = (pxConstList)->pxIndexd->pxNext;                 \
        /* 当下一个节点是尾节点时，继续向后一个节点 */        \
        if ((void *)(pxConstList)->pxIndex == (void *)&((pxConstList)->xListEnd)) \
        {                                                                         \
            (pxConstList)->pxIndex = (pxConstList)->pxIndex->pxNext;              \
        }                                                                         \
        /* 获取节点的OWNER，即TCB */                                       \
        (pxTCB) = (pxConstList)->pxIndex->pvOwner                                 \
    }

/**
 * @brief 初始化链表节点
 * @param pxItem 链表节点
 * @return void
 */
void vListInitialiseItem(ListItem_t *const pxItem);

/**
 * @brief 初始化链表
 * @param pxList 链表
 * @return void
 */
void vListInitialise(List_t *const pxList);

/**
 * @brief 将节点插入链表尾部
 * @param pxList 链表
 * @param pxNewListItem 链表节点
 */
void vListInsertEnd(List_t *const pxList, ListItem_t *const pxNewListItem);

/**
 * @brief 按升序将节点插入链表
 * @param pxList 链表
 * @param pxNewListItem 链表节点
 */
void vListInsert(List_t *const pxList, ListItem_t *const pxNewListItem);

/**
 * @brief 将节点从链表中删除
 * @param pxItemToRemove 要删除的节点
 * @return 链表中剩余的节点个数
 */
UBaseType_t uxListRemove(ListItem_t *const pxItemToRemove);