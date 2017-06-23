#include "huffman.h"

HuffmanNode * create_linked_list(const FreqVector & vec){
  HuffmanNode * head = NULL;
  HuffmanNode * node;

  for(FreqVector::const_reverse_iterator it=vec.rbegin(); it!=vec.rend(); it++) {
    node = new HuffmanNode;
    node->c = it->first;
    node->freq = it->second;
    node->next = head;
    node->left_child = NULL;
    node->right_child = NULL;
    head = node;
  }

  return head;
}

HuffmanNode * build_huffman_tree(HuffmanNode *head){
  HuffmanNode *n1, *n2, *newNode, *p, *trailing;

  while(head->next != NULL){
    n1 = head;
    n2 = head->next;
    head = n2->next;

    newNode = new HuffmanNode;
    newNode->freq = n1->freq + n2->freq;
    newNode->c = '\0';
    newNode->left_child = n1;
    newNode->right_child = n2;
    newNode->next = NULL;

    if(head != NULL && head->freq >= newNode->freq){
      // If the two smallest nodes are still smaller than the next node
      // then put the new node in front.
      newNode->next = head;
      head = newNode;
      print_list(head);
      continue;
    }
    p = head;
    trailing = NULL;
    while(true){
      if(p == NULL){
        // Reached the end of the linked list
        if(head == NULL)
          head = newNode;
        if(trailing != NULL)
          trailing->next = newNode;
        break;
      }
      if(p->freq >= newNode->freq){
        // Insert the new node here
        newNode->next = p;
        if(trailing != NULL)
          trailing->next = newNode;
        break;
      }else{
        // Check the next node
        trailing = p;
        p = p->next;
      }
    }
    print_list(head);
  }
  return head;
}

CodeTable generate_encodings(HuffmanNode *head){
  CodeTable encodings;
  Encoding base_encoding;
  traverse_tree(head, encodings, base_encoding);
  return encodings;
}

void traverse_tree(HuffmanNode *head, CodeTable &encodings, Encoding base_encoding){
  if(head->c == '\0'){
    base_encoding.push_back(true);
    traverse_tree(head->right_child, encodings, base_encoding);
    base_encoding.pop_back();
    base_encoding.push_back(false);
    traverse_tree(head->left_child, encodings, base_encoding);
    base_encoding.pop_back();
  }else{
    // Add character to encodings
    encodings[head->c] = base_encoding;
  }
}

Encoding encode_content(string text, CodeTable encodings){
  Encoding binary_encoding;
  for(int i=0; i<text.size(); i++){
    binary_encoding.insert(binary_encoding.end(), encodings[text[i]].begin(), encodings[text[i]].end());
  }
  cout << "\n\n";
  for(Encoding::iterator it=binary_encoding.begin(); it!=binary_encoding.end(); it++){
    cout << *it;
  }
  cout << "\n\n";
  return binary_encoding;
}

void write_file(Encoding binary_encoding, string filename){
  const std::size_t n = binary_encoding.size();
  char byteArray[(int)ceil(n/8.0)];

  for (std::size_t i = 0; i < n / 8; ++i)
    for (std::size_t j = 0; j < 8; ++j)
      if (binary_encoding[i * 8 + j] == true)
        byteArray[i] |= 1 << j;
  std::ofstream out(filename, std::ios::binary);
  out.write(byteArray, sizeof(byteArray));
}
