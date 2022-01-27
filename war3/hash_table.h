#pragma once

// wc3 hash algo
uint32_t string_hash(const char* str);

struct node;

struct node_2
{
	node_2* lft_;
	node* rht_;
	const char* str_;
};

struct node_1
{
	node_1* next_;
	node* prev_;
	node_2* lft_;
	node* rht_;
	const char* str_;
};

struct node
{
	uint32_t    hash_;
	node_1* next_;
	node* prev_;
	node_2* lft_;
	node* rht_;
	uint32_t    key;

	bool is_vaild() const
	{
		return ((intptr_t)this > 0x10000);
	}
};


template <class Node = node>
struct table
{
	template <class Node = node>
	struct entry
	{
		uint32_t step;
		node_1* tail;
		Node* head;

		node* convert(Node* ptr) const
		{
			return (node*)((uintptr_t)ptr + step - 4);
		}
	};

	uint32_t     unk0;
	uint32_t     step;
	uint32_t     tail;
	Node* head;
	uint32_t     unk4;
	uint32_t     unk5;
	uint32_t     unk6;
	entry<Node>* buckets;
	uint32_t     unk8;
	uint32_t     mask;

	Node* find(uint32_t hash)
	{
		Node* fnode_ptr = nullptr;

		if (mask == 0xFFFFFFFF)
			return nullptr;

		fnode_ptr = buckets[hash & mask].head;

		if (!fnode_ptr->is_vaild())
			return nullptr;

		for (;;)
		{
			if (fnode_ptr->hash_ == hash)
				return fnode_ptr;
			fnode_ptr = (Node*)(uintptr_t)(fnode_ptr->prev_);

			if (!fnode_ptr->is_vaild())
				return nullptr;
		}
	}

	Node* find(const char* str)
	{
		uint32_t hash;
		Node* fnode_ptr = nullptr;

		if (mask == 0xFFFFFFFF)
			return nullptr;

		hash = string_hash(str);

		fnode_ptr = buckets[hash & mask].head;

		if (!fnode_ptr->is_vaild())
			return nullptr;

		for (;;)
		{
			if (fnode_ptr->hash_ == hash)
			{
				if ((const char*)fnode_ptr->key == str)
					return fnode_ptr;

				if (0 == strcmp((const char*)fnode_ptr->key, str))
					return fnode_ptr;
			}
			fnode_ptr = (Node*)(uintptr_t)(fnode_ptr->prev_);

			if (!fnode_ptr->is_vaild())
				return nullptr;
		}
	}
};

struct reverse_node : public node
{
	uint32_t     value;
};

struct reverse_table
{
	typedef table<reverse_node> table_t;

	uint32_t            unk0_;
	uint32_t            size;
	reverse_node** node_array_;
	uint32_t            unk3_;
	table_t             table_;

	reverse_node* at(uint32_t index)
	{
		return node_array_[index];
	}

	reverse_node* find(uint32_t hash)
	{
		return table_.find(hash);
	}

	reverse_node* find(const char* str)
	{
		return table_.find(str);
	}
};
