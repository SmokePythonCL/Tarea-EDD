#include <string>
using namespace std;

class super_string {
    private:
        struct nodo {
            nodo *left = nullptr, *right = nullptr;
            int index;
            char c;
            nodo(int index, char c) : index(index), c(c) {}
            nodo(){}
        };
        int height = 0;       // Altura del árbol
        int length = 0;       // Largo del super-string
        nodo *root = nullptr; // Raíz del super-string
    public:
        super_string() {}
        ~super_string() {}
        void juntar(super_string &s);
        void agregar(char c); // Insertar un caracter en la última posición
        // En la izquierda esta el super_string a y en la derecha el super_string b
        void separar(int i, super_string &a, super_string &b);
        void reverso(); // No debe cambiar la altura del árbol
        int recortar(); // Retorna this->height después de recortar
        string stringizar(); // Debe ser O(n)
        void limpiar(); // Se deben borrar todos los nodos del super-string

        void inOrderStringizar(nodo* node, string& oracion);
        void inOrderReverso(nodo* node, super_string& temp);
        void inOrderSeparar(nodo* node, super_string& a, super_string& b, int i);
        void inOrderTransversal(nodo* node, nodo** nodes, int& i);
        super_string::nodo* buildBalancedTree(nodo** nodes, int start, int end);
        int Altura(nodo* node);
};

void super_string::agregar(char letra) {
    if (root == nullptr) {
        root = new nodo(0, letra);

        length++;
    } else {
        nodo *temp = root;
        while (temp->right != nullptr) {
            temp = temp->right;
        }
        temp->right = new nodo(length, letra);

        length++;
    }
}


string super_string::stringizar() {
    string oracion;
    inOrderStringizar(root, oracion);
    return oracion;
}

void super_string::separar(int i, super_string &a, super_string &b) {
    super_string temp;

    inOrderSeparar(a.root, temp, b, i);

    a.limpiar();
    if (temp.root != nullptr) {
        a.root = new nodo(0, temp.root->c);
        a.root->right = temp.root->right;
        a.length = temp.length;
    }
}

void super_string::limpiar() {
    if (root != nullptr) {
        nodo *temp = root;
        while (temp != nullptr) {
            nodo *temp2 = temp;
            temp = temp->right;
            delete temp2;
        }
        root = nullptr;
    }

    length = 0;
    height = 0;
}

void super_string::juntar(super_string &s) {
    if (s.root != nullptr) {
        if (length == 0) {
            root = new nodo(0, s.root->c);
            root->right = s.root->right;
            length = s.length;
            s.length = 0;
            s.root = nullptr;
        } else {
            nodo *temp = root;
            while (temp->right != nullptr) {
                temp = temp->right;
            }
            temp->right = s.root;
            length += s.length;
            s.length = 0;
            s.root = nullptr;
        }
    }
}

void super_string::reverso() {
    super_string temp;
    inOrderReverso(root, temp);
    limpiar();
    root = temp.root;
}

int super_string::recortar() {
    nodo** nodes = new nodo*[length];
    int i = 0;
    inOrderTransversal(root, nodes, i);
    root = buildBalancedTree(nodes, 0, length - 1);
    height = Altura(root);
    delete[] nodes;

    return height;
}

void super_string::inOrderStringizar(nodo* node, string& oracion) {
    if (node != nullptr) {
        inOrderStringizar(node->left, oracion);
        oracion += node->c;
        inOrderStringizar(node->right, oracion);
    }
}

void super_string::inOrderSeparar(nodo* node, super_string& a, super_string& b, int i) {
    if (node != nullptr) {
        inOrderSeparar(node->left, a, b, i);
        if (node->index < i) {
            a.agregar(node->c);
        } else if (node->index >= i){
            b.agregar(node->c);
        }
        inOrderSeparar(node->right, a, b, i);
    }
}

void super_string::inOrderReverso(nodo* node, super_string& temp) {
    if (node != nullptr) {
        inOrderReverso(node->right, temp);
        temp.agregar(node->c);
        inOrderReverso(node->left, temp);
    }
}

void super_string::inOrderTransversal(nodo* node, nodo** nodes, int& i) {
    if (!node) return;

    inOrderTransversal(node->left, nodes, i);
    nodes[i++] = node;
    inOrderTransversal(node->right, nodes, i);
}

super_string::nodo* super_string::buildBalancedTree(nodo** nodes, int start, int end) {
    if (start > end) return nullptr;

    int mid = (start + end) / 2;
    nodo* node = nodes[mid];

    node->left = buildBalancedTree(nodes, start, mid - 1);
    node->right = buildBalancedTree(nodes, mid + 1, end);

    return node;
}

int super_string::Altura(nodo* node) {
    if (!node) return 0;
    return 1 + max(Altura(node->left), Altura(node->right));
}