/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_LIST_H
#define ANT_LIST_H
#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_NEWTYPESDEFINE_H
#   include "Ant_NewTypesDefine.h"
#endif
#ifndef ANT_OBJECT_H
#   include "Ant_Object.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Class declare

/**
 * class Ant_List
 *
 * @brief a kind of linear sequence container which hold pointers in
 *
 * Ant_List is a kind of sequence container, its elements are ordered following a linear sequence.<br />
 * It keeps a pionter list to your data.
 *
 * @code
#ifndef ANT_LIST_H
#    include "Ant_List.h"
#endif

    Ant_List<INT> myList;
    myList<< new INT(1)<< new INT(2)<< new INT(3)<< new INT(4);    //List is 1,2,3,4

    cout<<myList.Length();
    // Output:4

    myList.InsertPos(new INT(5), 4);    //List is 1,2,3,4,5
    cout<<myList.Length();
    // Output:5

    INT* p = myList.Replace(new INT(6),0);    //List is 6,2,3,4,5
    delete p;

    Ant_List<ListItem>::Iterator it = myList.Begin();
    do
    {
        cout << *(it.Current());
    }
    while(NULL != it());
    // Output: 62345

    it.Reset();
    do
    {
        cout << *(it.Current());
        delete it.Current();    //Delete your elements(pointers)
    }
    while(NULL != it());
    // Output: 62345

    myList.Clear();        //Clear the list

 * @endcode
 *
 */
template <typename T>
class Ant_List : public virtual Ant_Object
{
public:

    friend class Iterator;

    class Node
    {
    public:

        Node(T* pt):prev(NULL), next(NULL), data(pt) {}

        Node*   prev;
        Node*   next;
        T*      data;

    private:

        Node(){}

    };

    /**
     * class Iterator
     *
     * @brief Iterator for Ant_List
     * @code
        Ant_List<INT> list;

        list<< new INT(1)<< new INT(1)<< new INT(3)<< new INT(2)<< new INT(1)
            << new INT(2)<< new INT(1)<< new INT(2)<< new INT(4)<< new INT(4)
            << new INT(3)<< new INT(4)<< new INT(1)<< new INT(3)<< new INT(2);

        Ant_List<INT>::Iterator it = list.Begin();

        //Delete 1
        do
        {
            if (1 == it.Current())
            {
                INT* p = it.RemoveCurrent();
                cout<<"Del:"<<*p<<"__";
                delete p;
            }
        }
        while(NULL != it());
        cout <<"/tlength:"<<list.Length();
        //Output:
        // Del:1__Del:1__Del:1__Del:1__Del:1__      length:10
     * @endcode
     */
    class Iterator : public virtual Ant_Object
    {
    public:

        Iterator()
        : m_pList(NULL), cur(NULL), fw(FALSE)
        {

        }

        Iterator(Ant_List *pList, BOOL bFw = TRUE)
        : m_pList(pList), fw(bFw)
        {
            if (fw) {
                cur = pList->first;
            }
            else {
                cur = pList->last;
            }
        }

        void set(Ant_List *pList, BOOL bFw = TRUE)
        {
            m_pList = pList;
            fw = bFw;
            reset();
        }

        /**
         * @brief Check wether an empty iterator
         *
         * @param VOID
         *
         * @return BOOL
         */
        BOOL isEmpty() const
        {
            return (m_pList == NULL);
        }

        /**
         * @brief Get current element
         * @code
            Ant_List<INT> myList;
            myList<< new INT(1)<< new INT(2)<< new INT(3)<< new INT(4);

            Ant_List<ListItem>::Iterator it = myList.Begin();
            do
            {
                cout << *(it.Current());
            }
            while(NULL != it());
            // Output: 1234
         * @endcode
         *
         * @param null
         *
         * @return T*
         */
        inline T* current() const
        {
            if (NULL == cur) {
                return NULL;
            }
            return cur->data;
        }

        /**
         * @brief Move Iterator(forward or backward)
         * @note Please use do {} while() for iterating
         *
         * @code
            Ant_List<INT> myList;
            myList<< new INT(1)<< new INT(2)<< new INT(3)<< new INT(4);

            Ant_List<ListItem>::Iterator it = myList.Begin();
            do
            {
                cout << *(it.Current());
            }
            while(NULL != it())

            // Output: 1234
         * @endcode
         *
         * @param null
         *
         * @return T*
         */
        Iterator* operator () ()
        {
            if (NULL == m_pList) {
                return NULL;
            }

            if (NULL != cur) {
                if (fw) {
                    cur = cur->next;
                }    
                else {
                    cur = cur->prev;
                }       

                if (NULL == cur) {
                    return NULL;
                }   // Move to the end.
                else {
                    return this;
                }
            }
            else {
                if (fw) {
                    cur = m_pList->first;
                }    
                else {
                    cur = m_pList->last;
                }       

                if (NULL == cur) {
                    return NULL;
                } 
                return this;
            }
        }

        /**
         * @brief reset iterator.
         * @note iterator will move to First if you create at Beginning of list, and move to Last if you create at End of list
         * @param VOID
         * @return VOID
         */
        VOID reset()
        {
            if (NULL == m_pList) {
                return;
            }
            if (fw) {
               cur = m_pList->first;
            }
            else {
                cur = m_pList->last;
            }
        }

        /**
         * @brief remove current element in list.
         * @param VOID
         * @return T* node be removed.
         */
        T* removeCurrent()
        {
            if (NULL == cur) {
                return NULL;
            }

            Node* tmp = cur;
            if (fw) {
                cur = tmp->prev;
            }
            else {
                cur = tmp->next;
            }

            T* data = tmp->data;
            m_pList->deleteNode(tmp);
            return data;
        }

        /**
         * @brief remove current element in list and delete it.
         * @param VOID
         * @return VOID
         */
        VOID deleteCurrent()
        {
            if (NULL == cur) {
                return;
            }

            Node* tmp = cur;
            if (fw) {
                cur = tmp->prev;
            }
            else {
                cur = tmp->next;
            }

            T* data = tmp->data;
            m_pList->deleteNode(tmp);
            delete data;
        }

        /**
         * @brief replace current element in list.
         * @param VOID
         * @return T* node be replaced.
         */
        T* replaceCurrent(T* t)
        {
            if (NULL == cur) {
                return NULL;
            }
            T* data = cur->data;
            cur->data = t;
            return data;
        }

    private:
        Ant_List*    m_pList;
        Node*        cur;
        BOOL        fw;    // Forward?
    };

public:

    Ant_List():
    first(NULL),
    last(NULL),
    size(0) 
    {

    }

    virtual ~Ant_List() 
    {
        clear();
    }

public:

    /**
     * @code
        Ant_List<INT> myList;
        if (0 == myList.Length())
        {
            //Yeah, it is 0.
        }
     * @endcode
     *
     * @param null
     *
     * @return INT
     */
    /// Get length of current list
    inline INT length() const {return size;}

    /**
     * @code
        Ant_List<INT> myList;
        if (!myList.HasData())
        {
            //it has no data.
        }
     * @endcode
     *
     * @param null
     *
     * @return BOOL
     */
    /// Does current list have data?
    inline BOOL hasData() const {return 0 != size;}

    /**
     * @note 1.If dst is NULL, Add() is same as Append() <br />2.If "dst" is not in the List, Add() will return FALSE and "t" will not be added into list
     *
     * @code
        Ant_List<INT> myList;
        INT* p1  = new INT (1);
        INT* p2  = new INT (2);

        myList.Add(p1);
        myList.Add(p2,p1);

        cout<< *myList[0] << *myList[1];
        //Output: 12
     * @endcode
     *
     * @param T*  t
     *
     * @param const T*  dst = NULL
     *
     * @return BOOL
     */
    ///Add pointer "t" after pointer "dst" in the list
    BOOL add(T* t, const T* dst = NULL);

    /**
     * @note 1.If dst is NULL, Add() is same as Push() <br />2.If "dst" is not in the List, Insert() will return FALSE and "t" will not be inserted into list
     *
     * @code
        Ant_List<INT> myList;
        INT* p1  = new INT (1);
        INT* p2  = new INT (2);

        myList.Insert(p1);
        myList.Insert(p2,p1);

        cout<< *myList[0] << *myList[1];
        //Output: 21
     * @endcode
     *
     * @param T*  t
     *
     * @param const T*  dst = NULL
     *
     * @return BOOL
     */
    /// Insert pointer "t" before poniter "dst" in the list,
    BOOL insert(T* t, const T* dst = NULL);

    /**
     * @note If pos is greater than the length of list, this IF will fail Inserting element and return FALSE.
     *
     * @code
        Ant_List<INT> myList;
        myList.Insert(new INT(10), 2);    // Failed, 2(pos) > 0(length)
        myList<< new INT(1)<< new INT(2)<< new INT(3)<< new INT(4);
        myList.Insert(new INT(10), 3);    //Succeed 3(pos) < 4(length)
        myList.Insert(new INT(10), 4);    //Succeed 4(pos) = 4(length)
     * @endcode
     *
     * @param T*  t
     *
     * @param INT pos = 0
     *
     * @return BOOL
     */
    /// Insert pointer "t" at pos
    BOOL insertPos(T* t, INT pos = 0);

    /**
     * @note When pos is invalid, Nth(i) will return NULL
     *
     * @code
        Ant_List<INT> myList;

        INT* a = new INT(1);
        INT* b = new INT(2);
        INT* c = new INT(3);

        myList<<a<<b<<c;

        cout<<*myList.Nth(0)<<*myList.Nth(1)<<*myList.Nth(2);
        //output: 123
        INT * invalidPtr = myList.Nth(6);
        // invalidPtr == NULL
     * @endcode
     *
     * @param INT pos
     *
     * @return T*
     */
    /// Get the element at [pos]
    T* nth(INT pos) const;

    /**
     *@note If List is empty, GetLast() returns NULL
     *
     *@code
        Ant_List<INT> myList;

        INT* a = new INT(1);
        INT* b = new INT(2);
        INT* c = new INT(3);

        myList<<a<<b<<c;

        cout<<*myList.GetLast();
        //output: 3
     * @endcode
     *
     * @param null
     *
     * @return T*
     */
    /// Get the last element
    T* getLast() const;

    /**
     * @code
        Ant_List<INT> myList;

        myList.Append(new INT(1));
        myList.Append(new INT(2));

        cout<< *myList[0] << *myList[1];
        //Output: 12
     * @endcode
     *
     * @param T*  t
     *
     * @return VOID
     */
    /// Append pointer "t" at the end of current List
    VOID append(T* t);

    /**
     * @code
        Ant_List<INT> myList;
        Ant_List<INT> hisList;

        myList<< new INT(1)<< new INT(2)<< new INT(3)<< new INT(4);
        hisList<< new INT(5)<< new INT(6)<< new INT(7)<< new INT(8);

        hisList.Append(myList);

        for (INT i = 0; i < hisList.Length(); i++)
        {
            cout << *hisList[i];
        }
        //output: 56781234
     * @endcode
     *
     * @param Ant_List<T>& t
     *
     * @return VOID
     */
    /// append a List "t" at the end of current List
    VOID append(Ant_List<T> &t);

    /**
     * @code
        Ant_List<INT> myList;

        myList.Push(new INT(1));
        myList.Push(new INT(2));

        cout<< *myList[0] << *myList[1];
        //Output: 21
     * @endcode
     *
     * @param T*  t
     *
     * @return VOID
     */
    /// Push pointer "t" at the begining of current List
    VOID push(T* t);

    /**
     * @code
        Ant_List<INT> myList;
        Ant_List<INT> hisList;

        myList<< new INT(1)<< new INT(2)<< new INT(3)<< new INT(4);
        hisList<< new INT(5)<< new INT(6)<< new INT(7)<< new INT(8);

        hisList.Push(myList);

        for (INT i = 0; i < hisList.Length(); i++)
        {
            cout << *hisList[i];
        }
        //output: 12345678
     * @endcode
     *
     * @param Ant_List<T>& t
     *
     * @return VOID
     */
    /// Push a List at the begining of current List
    VOID push(Ant_List<T> &t);

    /**
     * @note If pos is invalid, Replace() will fail, and return value is NULL
     *
      * @code
        Ant_List<INT> myList;
        myList<< new INT(1)<< new INT(2)<< new INT(3)<< new INT(4);
        cout << *myList.Replace(10, 3);    //Succeed 3(pos) < 4(length)
        // output: 4

        cout << *myList.Replace(10, 4);        //Failed,        4(pos) = 4(length), and this will return NULL
        // this will cause Crush(*NULL)
     * @endcode
     *
     * @param T*  t
     *
     * @param INT pos
     *
     * @return T*
     */
    /// Replace the element at [pos] with "t" and return the original one
    T* replace(T* t, INT pos);

    /**
     * @note If list is empty, Pop() returns NULL
     *
      * @code
        Ant_List<INT> myList;
        myList<< new INT(1)<< new INT(2)<< new INT(3)<< new INT(4);
        cout << *myList.Pop();
        // output: 1

        cout << myList.Length();
        // output:3
     * @endcode
     *
     * @param null
     *
     * @return T*
     */
    /// Pop first element away from list
    T* pop();

    /**
     * @note if pos is invalid, TakeAt() fails and returns NULL
     *
      * @code
        Ant_List<INT> myList;
        myList<< new INT(1)<< new INT(2)<< new INT(3)<< new INT(4);
        cout << *myList.TakeAt(3);
        // output: 4

        cout << myList.Length();
        // output:3
     * @endcode
     *
     * @param INT pos
     *
     * @return T*
     */
    /// Take the element away at pos from list(element will be deleted from list)
    T* takeAt(INT pos);

    /**
     * @note if pos is invalid, TakeAt() fails and returns NULL
     *
      * @code
        Ant_List<INT> myList;
        INT* p = new INT(1);
        myList<< p;
        delete (myList.Take(p));

        cout << myList.Length();
        // output:0
     * @endcode
     *
     * @param const T* t
     * @param BOOL fw = TRUE fw:TRUE, search forward, FALSE, search backward
     *
     * @return T*
     */
    /// Take the specfied element away from list(element will be deleted from list)
    T* take(const T* t, BOOL fw = TRUE);

    /**
     * @note If list is empty, TakeFirst() returns NULL
     *
      * @code
        Ant_List<INT> myList;
        myList<<new INT(1)<< new INT(2)<< new INT(3)<< new INT(4);
        cout << *myList.TakeFirst();
        // output: 1

        cout << myList.Length();
        // output:3
     * @endcode
     *
     * @param null
     *
     * @return T*
     */
    /// take first element away from list
    T* takeFirst();

    /**
     * @note If list is empty, TakeLast() returns NULL
     *
      * @code
        Ant_List<INT> myList;
        myList<<new INT(1)<< new INT(2)<< new INT(3)<< new INT(4);
        cout << *myList.TakeLast();
        // output: 4

        cout << myList.Length();
        // output:3
     * @endcode
     *
     * @param null
     *
     * @return T*
     */
    /// take last element away from list
    T* takeLast();

    /**
     * @note Clear() only delete all the node saved in the list, but don't delete your elements
     *
      * @code
        Ant_List<INT> myList;
        myList<< new INT(1)<< new INT(2)<< new INT(3)<< new INT(4);
        for (INT i = 0; i < myList.Length(); i++)
        {
            delete myList[i];    // you have to delete your elements yourself
        }
        myList.Clear();

        cout << myList.Length();
        // output:0
     * @endcode
     *
     * @param null
     *
     * @return VOID
     */
    /// Clear the list
    VOID clear();

    /**
     * @note ClearData() only delete all the node saved in the list, and delete your elements
     *
      * @code
        Ant_List<INT> myList;
        myList<< new INT(1)<< new INT(2)<< new INT(3)<< new INT(4);
        myList.ClearData();

        cout << myList.Length();
        // output:0
     * @endcode
     *
     * @param null
     *
     * @return VOID
     */
    /// Clear the list and delete all elements
    VOID clearData();

    // Iterator

    /**
      * @note Begin() will set the moving direction of Iterator: Forward<br/>Please don't create an Iterator in an empty list
     *
     * @code
        Ant_List<INT> myList;
        myList<< new INT(1)<< new INT(2)<< new INT(3)<< new INT(4);
        Ant_List<INT>::Iterator it = myList.Begin();
        do
        {
            cout<<*(it.Current());
        }
        while(NULL != it())
        //Output:1234
     * @endcode
     *
     * @param null
     *
     * @return Iterator
     */
    /// Get Iterator witch pointed to the beginning of the list
    Iterator begin() {return Iterator(this);}

    /**
      *     @note End() will set the moving direction of Iterator: backward<br/>Please don't create an Iterator in an empty list
     *
     * @code
        Ant_List<INT> myList;
        myList<< new INT(1)<< new INT(2)<< new INT(3)<< new INT(4);

        Ant_List<INT>::Iterator it = myList.End();
        do
        {
            cout<<*(it.Current());
        }
        while(NULL != it())
        //Output:4321
     * @endcode
     *
     * @param null
     *
     * @return Iterator
     */
    /// Get Iterator witch pointed to the end of the list
    Iterator end() {return Iterator(this, FALSE);}

    /**
      * @note If fw=TRUE, the Iterator of the first same data with the specified value in list will be returned<br/>If dw=FALSE, the last's Iterator will be returned<br/>An empty iterator will be returned if the value cannot be found
      *
      * @code
        Ant_List<INT> myList;
        INT *p1 = new INT(1);
        INT *p2 = new INT(2);
        INT *p3 = new INT(3);
        INT *p3 = new INT(4);
        myList << p1 << p2 << p3 << p2 << p4;

        Ant_List<INT>::Iterator it = myList.Find(p2);
        if (!it.IsEmpty())
        {
            it.RemoveCurrent();
        }
        //Output:4231
     * @endcode
     *
     * @param const T* t
     * @param BOOL fw = TRUE fw:TRUE, search forward, FALSE, search backward
     *
     * @return Iterator
     */
    /// Get Iterator witch pointed to the specified value
    Iterator find(const T* t, BOOL fw = TRUE);

public:

    /**
     * @code
        Ant_List<INT> myList;
        myList<< new INT(1)<< new INT(2)<< new INT(3)<< new INT(4);

        Ant_List<INT> hisList(myList);

        myList.Clear();
        cout << hisList.Length();
        // Output: 4
     * @endcode
     *
     * @param Ant_List<T>& list
     *
     * @return Ant_List<T>&
     */
    /// evaluate the old List to a new list
    Ant_List(const Ant_List<T> &list);

    /**
     * @code
        Ant_List<INT> myList;
        myList<< new INT(1)<< new INT(2)<< new INT(3)<< new INT(4);

        Ant_List<INT> hisList;
        hisList = myList;

        myList.Clear();
        cout << hisList.Length();
        // Output: 4
     * @endcode
     *
     * @param Ant_List<T>& list
     *
     * @return Ant_List<T>&
     */
    /// evaluate the old List to a new list
    Ant_List<T>& operator = (const Ant_List<T> &list);

    /**
     * @code
        Ant_List<INT> myList;
        myList<< new INT(1)<< new INT(2)<< new INT(3)<< new INT(4);

        cout << *myList[0] << *myList[1] << *myList[2] << *myList[3];
        // Output: 1234
     * @endcode
     *
     * @param INT pos
     *
     * @return T*
     */
    /// Get element operator, same as Ant_List<T>::Nth()
    inline T* operator [] (INT pos) const {return nth(pos);}

    /**
     * @code
        Ant_List<INT> myList;
        myList<< new INT(1)<< new INT(2)<< new INT(3)<< new INT(4);

        cout << *myList[0] << *myList[1] << *myList[2] << *myList[3];
        // Output: 1234
     * @endcode
     *
     * @param T*  t
     *
     * @return Ant_List<T>&
     */
    /// Append element operator, same as Ant_List<T>::Append(T* t)
    inline Ant_List<T>& operator << (T* t) {append(t); return *this;}

    /**
     * @code
        Ant_List<INT> myList;
        myList<< new INT(1)<< new INT(2)<< new INT(3)<< new INT(4);

        Ant_List<INT> hisList;
        hisList<<myList;

        cout << *hisList[0] << *hisList[1] << *hisList[2] << *hisList[3];
        // Output: 1234
     * @endcode
     *
     * @param Ant_List<T>&  t
     *
     * @return Ant_List<T>&
     */
    /// Append list operator, same as Ant_List<T>::Append(const Ant_List<T> &t)
    inline Ant_List<T>& operator << (Ant_List<T>& t) {append(t); return *this;}

protected:

    VOID deleteNode(Node* pNode);

protected:
    Node*   first;
    Node*   last;
    INT     size;
};

/********************************************************************
        Implement...

********************************************************************/

template <typename T>
T* Ant_List<T>::nth(INT pos) const
{
    if (pos >= size || 0 > pos) {
        return NULL;
    }

    Node* pNode = first;
    for (INT i = 0; i < pos; i ++) {
        pNode = pNode->next;
    }
    return pNode->data;
}

template <typename T>
T* Ant_List<T>::getLast() const
{
    if (NULL == last) {
        return NULL;
    }

    return last->data;
}

template <typename T>
BOOL Ant_List<T>::add(T* t, const T* dst)
{
    if (NULL == dst) {
        append(t);
        return TRUE;
    }

    BOOL bFind = FALSE;
    Node* pNode = first;
    while(NULL != pNode) {
        if (dst == pNode->data) {
            bFind = TRUE;
            break;
        }
        else {
            pNode = pNode->next;
        }
    }

    if (bFind) {
        if (last == pNode) {
            append(t);
        }
        else {
            Node* insertNode = new Node(t);
            pNode->next->prev = insertNode;
            insertNode->next = pNode->next;
            insertNode->prev = pNode;
            pNode->next = insertNode;
            size++;
        }
    }

    return bFind;
}

template <typename T>
BOOL Ant_List<T>::insert(T* t, const T* dst)
{
    if (NULL == dst) {
        push(t);
        return TRUE;
    }

    BOOL bFind = FALSE;
    Node* pNode = first;
    while(NULL != pNode) {
        if (dst == pNode->data) {
            bFind = TRUE;
            break;
        }
        else {
            pNode = pNode->next;
        }
    }

    if (bFind) {
        if (first == pNode) {
            push(t);
        }
        else {
            Node* insertNode = new Node(t);
            pNode->prev->next = insertNode;
            insertNode->prev = pNode->prev;
            insertNode->next = pNode;
            pNode->prev = insertNode;
            size++;
        }
    }

    return bFind;
}

template <typename T>
VOID Ant_List<T>::append(T* t)
{
    Node* pNode = new Node(t);

    if (0 == size) {
        first = pNode;
        last = pNode;
    }
    else {
        last->next = pNode;
        pNode->prev = last;
        last = pNode;
    }

    size++;
}

template <typename T>
VOID Ant_List<T>::append(Ant_List<T> &t)
{
    for (INT i=0; i < t.length(); i++) {
        append(t[i]);
    }
}

template <typename T>
VOID Ant_List<T>::push(T* t)
{
    Node* pNode = new Node(t);

    if (0 == size) {
        first = pNode;
        last = pNode;
    }
    else {
        first->prev = pNode;
        pNode->next = first;
        first = pNode;
    }

    size++;
}

template <typename T>
VOID Ant_List<T>::push(Ant_List<T> &t)
{
    INT total = t.Length();
    for (INT i=1; i <= total; i++) {
        Push(t[total - i]);
    }
}

template <typename T>
BOOL Ant_List<T>::insertPos(T* t, INT pos)
{
    // Pos is invalid, drop it.
    if (pos > size || 0 > pos) {
        return FALSE;
    }

    if (0 == pos) {
        push(t);
    }
    else if (pos == size) {
        append(t);
    }
    else {
        Node* pNode = first;
        for (INT i = 0; i < pos; i++) {
            pNode = pNode->next;
        }

        Node* pInsertNode = new Node(t);

        pInsertNode->next = pNode;
        pInsertNode->prev = pNode->prev;
        pNode->prev->next = pInsertNode;
        pNode->prev = pInsertNode;

        size++;
    }

    return TRUE;
}

template <typename T>
T* Ant_List<T>::replace(T* t, INT pos)
{
    if (pos >= size || 0 > pos) {
        return NULL;
    }

    Node* pNode = first;
    for (INT i = 0; i < pos; i++) {
        pNode = pNode->next;
    }

    T* rtn = pNode->data;
    pNode->data = t;

    return rtn;
}

template <typename T>
T* Ant_List<T>::pop()
{
    return takeFirst();
}

template <typename T>
T* Ant_List<T>::takeAt(INT pos)
{
    // pos invalid, take the first one
    if (pos >= size || 0 > pos) {
        return NULL;
    }
    else if (0 == pos) {
        return takeFirst();
    }
    else if (pos == size - 1) {
        return takeLast();
    }
    else {
        Node* pNode = first;
        for (INT i = 0; i < pos; i++) {
            pNode = pNode->next;
        }

        T* data = pNode->data;
        deleteNode(pNode);
        return data;
    }
}

template <typename T>
T* Ant_List<T>::take(const T* t, BOOL fw)
{
    Node* pNode = last;
    if (fw) {
        pNode = first;
    }

    while (NULL != pNode) {
        if (pNode->data == t) {
            T* data = pNode->data;
            deleteNode(pNode);
            return data;
        }

        if (fw) {
            pNode = pNode->next;
        }
        else {
            pNode = pNode->prev;
        }
    }
    return NULL;
}

template <typename T>
T* Ant_List<T>::takeFirst()
{
    if (0 == size) {
        return NULL;
    }

    T* t = first->data;
    deleteNode(first);
    return t;
}

template <typename T>
T* Ant_List<T>::takeLast()
{
    if (0 == size) {
        return NULL;
    }

    T* t = last->data;
    deleteNode(last);
    return t;
}

template <typename T>
VOID Ant_List<T>::clear()
{
    Node* pNode = first;
    while(NULL != pNode) {
        Node* tmp = pNode;
        pNode = pNode->next;
        delete tmp;
    }

    first = NULL;
    last = NULL;
    size = 0;
}

template <typename T>
VOID Ant_List<T>::clearData()
 {
    Node* pNode = first;
    while(NULL != pNode) {
        Node* tmp = pNode;
        pNode = pNode->next;
        delete tmp->data;
        delete tmp;
    }

    first = NULL;
    last = NULL;
    size = 0;
}

template <typename T>
typename Ant_List<T>::Iterator Ant_List<T>::find(const T* t, BOOL fw)
{
    if (0 == size) {
        return Iterator();
    }

    Iterator it(this, fw);

    do {
        if (it.Current() == t) {
            return it;
        }
    } while (NULL != it());

    return Iterator();
}

template <typename T>
VOID Ant_List<T>::deleteNode(Node* pNode)
{
    if (0 == size || NULL == pNode) {
        return;
    }

    if (pNode == first) {
        if (1 == size) {
            first = NULL;
            last = NULL;
            delete pNode;
        }
        else {
            first = pNode->next;
            first->prev = NULL;
            delete pNode;
        }
    }
    else if (pNode == last) {
        if (1 == size) {
            first = NULL;
            last = NULL;
            delete pNode;
        }
        else {
            last = pNode->prev;
            last->next = NULL;
            delete pNode;
        }
    }
    else {
        pNode->prev->next = pNode->next;
        pNode->next->prev = pNode->prev;
        delete pNode;
    }

    size--;
}

template <typename T>
Ant_List<T>::Ant_List(const Ant_List<T> &list)
:first(NULL),
last(NULL),
size(0)
{
    for (INT i = 0; i < list.length(); i++) {
        append(list[i]);
    }
}

template <typename T>
Ant_List<T>& Ant_List<T>::operator = (const Ant_List<T> &list)
{
    if (this == &list) {
        return *this;
    }

    for (INT i = 0; i < list.length(); i++) {
        append(list[i]);
    }

    return *this;
}

template <typename T>
class Ant_ObjList : public Ant_List<T>
{
public:

    virtual ~Ant_ObjList(){Ant_List<T>::clearData();}

    T& append();

    T& push();
};

template <typename T>
T& Ant_ObjList<T>::append()
{
    T* t = new T();
    Ant_List<T>::append(t);
    return *t;
}

template <typename T>
T& Ant_ObjList<T>::push()
{
    T* t = new T();
    Ant_List<T>::push(t);
    return *t;
}

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif  // ANT_LIST_H

/*---------------------------------------------------------------------------*/
/* EOF */
