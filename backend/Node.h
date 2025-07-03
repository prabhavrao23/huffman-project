    #ifndef NODE_H
    #define NODE_H

    #include <string> // Add this line!

    struct Node {

        Node(char letter, int weight) {
            this->letter = letter;
            this->weight = weight;
            this->zero = nullptr;
            this->one = nullptr;
            this->code = ""; // Ensure 'code' is declared as std::string
        }

        Node(char letter, int weight, Node *zero, Node *one) {
            this->letter = letter;
            this->weight = weight;
            this->zero = zero;
            this->one = one;
            this->code = ""; // Ensure 'code' is declared as std::string
        }

        char letter;         // letter character to be stored
        Node* zero;
        Node* one;
        int weight;          // count for how many times the character is used in the file
        std::string code;    // binary string code created from the huffman tree (ensure std::string)
    };


    #endif //NODE_H


    #ifndef COMPARE_WEIGHTS
    #define COMPARE_WEIGHTS

    struct compareWeights{
        bool operator()(Node* const& lhs, Node* const& rhs){
            return lhs->weight > rhs->weight;
        }
    };
    #endif // COMPARE_WEIGHTS
    