using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace hrd27
{
    public class LList<T>
    {

        private class Node
        {
            public T? Data { get; set; }
            public Node? Next { get; set; }

            public Node(T? data, Node? next)
            {
                Data = data;
                Next = next;
            }

            public Node() : this(default, null) { }
        }

        private Node? m_head;
        private int m_size;

        public void Push(T item_)
        {
            Node newNode = new Node(item_, m_head);
            m_head = newNode;
            m_size++;
        }
    }

    public class ThisStack<T>
    {
        private T[] m_items;
        private int m_size;

        public ThisStack(int cap_)
        {
            m_items = new T[cap_];
            m_size = 0;
        }

        public ThisStack() : this(10) { }

        public void Push(T item_)
        {
            if (m_size == m_items.Length)
            {
                Array.Resize(ref m_items, m_items.Length * 2);
            }

            m_items[m_size] = item_;
            ++m_size;
        }

        public void Pop()
        {
            if (0 == m_size)
            {
                throw new InvalidOperationException("Stack is empty");
            }

            --m_size;
            T item = m_items[m_size];

            m_items[m_size] = default(T);

        }

        public T Peek()
        {
            if (0 == m_size)
            {
                throw new InvalidOperationException("Stack is empty");
            }

            return m_items[m_size];
        }

        public bool IsEmpty()
        {
            return m_size == 0;
        }

        public void Clear()
        {
            Array.Clear(m_items, 0, m_size);
            m_size = 0;
        }
    }
}
