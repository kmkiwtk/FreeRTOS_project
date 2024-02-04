#include <portmacro.h>
#include <list.h>

void vListInitialiseItem(ListItem_t *const pxItem)
{
    /*初始化该节点所在的链表为空，表示该节点还没有插入任何链表*/
    pxItem->pvContainer = NULL;
}

void vListInitialise(List_t *const pxList)
{
    /* 将链表索引指针指向最后一个节点 */
    pxList->pxIndex = (ListItem_t *)&(pxList->xListEnd);

    /* 将链表随后一个节点的辅助值设置为最大，确保该节点就是链表的最后节点 */
    pxList->xListEnd.xItemValue = portMAX_DELAY;

    /* 将最后一个节点的pxNext和pxPrevious指针均指向节点自身，表示链表为空 */
    pxList->xListEnd.pxNext = (ListItem_t *)&(pxList->xListEnd);
    pxList->xListEnd.pxPrevious = (ListItem_t *)&(pxList->xListEnd);

    /* 初始化链表节点计数器的值为0，表示链表为空 */
    pxList->uxNumberOfItems = (UBaseType_t)0U;
}

void vListInsertEnd(List_t *const pxList, ListItem_t *const pxNewListItem)
{
    ListItem_t *const pxIndex = pxList->pxIndex;

    pxNewListItem->pxNext = pxIndex;
    pxNewListItem->pxPrevios = pxIndex->pxPrevios;
    pxIndex->pxPrevios->pxNext = pxNewListItem;
    pxIndex->pxPrevios = pxNewListItem;

    /* 记住该节点所在的链表 */
    pxNewListItem->pvContainer = (void *)pxList;

    /* 链表节点计数器++ */
    (pxList->uxNumberOfItems)++;
}

void vListInsert(List_t *const pxList, ListItem_t *const pxNewListItem)
{
    ListItem_t *pxIterator;

    /* 获取节点的排序辅助值 */
    const TickType_t xValueofInsertion = pxNewListItem->xItemValue;

    /* 寻找节点要插入的位置 */
    if (xValueofInsertion == portMAX_DELAY)
    {
        pxIterator = pxList->xListEnd.pxPrevious;
    }
    else
    {
        for (pxIterator = (ListItem_t *)&(pxList->xListEnd);
             pxIterator->pxNext->xItemValue <= xValueofInsertion;
             pxIterator = pxIterator->pxNext)
        {
            /* 没有事情可做，不断迭代只为了找到节点要插入的位置*/
        }
    }
    /* 根据升序排列，将节点插入 */
    pxNewListItem->pxNext = pxIterator->pxNext;
    pxNewListItem->pxNext->pxPrevios = pxNewListItem;
    pxNewListItem->pxPrevios = pxIterator;
    pxIterator->pxNext = pxNewListItem;

    /* 记住该节点所在的链表 */
    pxNewListItem->pvContainer = (void *)pxList;

    /* 链表节点计数器++ */
    (pxList->uxNumberOfItems)++;
}

UBaseType_t uxListRemove(ListItem_t *const pxItemToRemove)
{
    /* 获取节点所在的链表 */
    List_t *const pxList = (List_t *)pxItemToRemove->pvContainer;
    /* 将指定的节点从链表删除 */
    pxItemToRemove->pxNext->pxPrevios = pxItemToRemove->pxPrevios;
    pxItemToRemove->pxPrevios->pxNext = pxItemToRemove->pxNext;

    pxItemToRemove->pxNext = NULL;
    pxItemToRemove->pxPrevios = NULL;

    /* 调整链表的索引指针 */
    if (pxList->pxIndex == pxItemToRemove)
    {
        pxList->pxIndex = pxItemToRemove->pxPrevios;
    }

    /* 初始化该节点所在的链表为空，表示节点还没有插入任何链表 */
    pxItemToRemove->pvContainer = NULL;

    /* 链表节点计数器-- */
    (pxList->uxNumberOfItems)--;

    /* 返回链表中剩余节点的个数 */
    return pxList->uxNumberOfItems;
}