
template<class T>
Conjunto<T>::Conjunto() {
    _cardinal = 0;
    _raiz = nullptr;
}

template<class T>
Conjunto<T>::~Conjunto() {
    limpiar(_raiz);
}

template<class T>
void Conjunto<T>::limpiar(Conjunto<T>::Nodo *nodo) {
    if (nodo != nullptr) {
        //Borro el lado izquierdo del nodo
        if (nodo->izq != nullptr) limpiar(nodo->izq);
        //Borro el lado derecho del nodo
        if (nodo->der != nullptr) limpiar(nodo->der);
        //Borro el nodo
        delete nodo;
    }
}

template<class T>
bool Conjunto<T>::pertenece(const T &clave) const {
    return (_raiz != nullptr && bajar(_raiz, clave)->valor == clave);
}

template<class T>
void Conjunto<T>::insertar(const T &clave) {

    //Busco el futuro padre
    Nodo *futuroPadre = bajar(_raiz, clave);

    //Si no hay raiz, insercion inicial
    if (futuroPadre == nullptr) {
        _raiz = new Nodo(clave);
        _cardinal++;
    }
        //Si no, tengo que insertar en algun nodo
    else {
        //Si es menor, lo inserto a la izquierda
        if (clave < futuroPadre->valor)
            futuroPadre->izq = new Nodo(clave);
            //Si es mayor, lo inserto a la derecha
        else if (clave > futuroPadre->valor)
            futuroPadre->der = new Nodo(clave);

        //Si el futuro padre no es si mismo, se inserto un elemento. Aumento el cardinal
        if (futuroPadre->valor != clave) _cardinal++;
    }
}

template<class T>
typename Conjunto<T>::Nodo *Conjunto<T>::bajar(Nodo *nodo, const T &v) const {
    if (_raiz == nullptr) return nullptr;
    //Si la raiz es menor esta del lado izquierdo
    if (nodo->izq != nullptr && nodo->valor > v)
        return bajar(nodo->izq, v);

        //Si la raiz es mayor esta del lado derecho
    else if (nodo->der != nullptr && nodo->valor < v)
        return bajar(nodo->der, v);

        //Si no tiene ni izquierda ni derecha, es el nodo mas cercano al valor
    else return nodo;
}

template<class T>
void Conjunto<T>::remover(const T &clave) {
    Nodo *supuestoNodo = bajar(_raiz, clave);
    //Si es vacio no hago nada
    if (_raiz == nullptr || supuestoNodo->valor != clave) return;

    //Si tiene un solo elemento, lo eliminio
    if (_raiz->der == nullptr && _raiz->izq == nullptr) {
        delete (_raiz);
        _raiz = nullptr;
        _cardinal--;
    }
        //Si tiene mas de un elemento piso el nodo con el arbol sin ese nodo
    else {
        Nodo *nuevo_subarbol = remover_subarbol(_raiz, clave);
        //Si el eliminado fue la raiz cambio la raiz a la del nuevo arbol
        if (supuestoNodo == _raiz)
            _raiz = nuevo_subarbol;
        supuestoNodo = nuevo_subarbol;
        _cardinal--;
    }
}

template<class T>
typename Conjunto<T>::Nodo *Conjunto<T>::remover_subarbol(Conjunto<T>::Nodo *raiz, const T &v) const {
    //Caso base
    if (raiz == nullptr) return raiz;

    //Si es menor a la raiz, esta en la izquierda
    if (v < raiz->valor)
        raiz->izq = remover_subarbol(raiz->izq, v);

        //Si es mayor... a la derecha
    else if (v > raiz->valor)
        raiz->der = remover_subarbol(raiz->der, v);

        //Si es igual, entonces es el que hay que eliminar
    else {
        //Si tiene un solo hijo, o no tiene hijo
        if (raiz->izq == nullptr) {
            Nodo *swap = raiz->der;
            delete (raiz);
            return swap;
        } else if (raiz->der == nullptr) {
            Nodo *swap = raiz->izq;
            delete (raiz);
            return swap;
        }

        //Si tiene dos hijos
        //Obtengo el menor de l aderecha para swapear
        Nodo *swap = minimo_subarbol(raiz->der);
        //Swapeo: copio el valor del minimo de la derecha donde esta el que quiero eliminar
        raiz->valor = swap->valor;
        //Borro el valor que swapie
        raiz->der = remover_subarbol(raiz->der, swap->valor);
    }

    return raiz;
}

template<class T>
const T &Conjunto<T>::siguiente(const T &clave) {
    Nodo *nodo = bajar(_raiz, clave);
    return siguiente_nodo(nodo)->valor;

}

template<class T>
typename Conjunto<T>::Nodo *Conjunto<T>::siguiente_nodo(Conjunto<T>::Nodo *nodo) const {
    if (nodo->der != nullptr)
        return minimo_subarbol(nodo->der);

    Nodo *n = nullptr;
    Nodo *r = _raiz;

    while (r != nullptr) {
        if (nodo->valor < r->valor) {
            n = r;
            r = r->izq;
        } else if (nodo->valor > r->valor)
            r = r->der;
        else
            break;
    }

    return n;
}


template<class T>
const T &Conjunto<T>::minimo() const {
    return minimo_subarbol(_raiz)->valor;
}

template<class T>
typename Conjunto<T>::Nodo *Conjunto<T>::minimo_subarbol(Conjunto<T>::Nodo *raiz) const {
    while (raiz->izq != nullptr)
        raiz = raiz->izq;
    return raiz;
}

template<class T>
const T &Conjunto<T>::maximo() const {
    Nodo *nodo = _raiz;
    while (nodo->der != nullptr)
        nodo = nodo->der;
    return nodo->valor;
}

template<class T>
unsigned int Conjunto<T>::cardinal() const {
    return _cardinal;
}

template<class T>
void Conjunto<T>::mostrar(std::ostream &s) const {
    s << "{";
    T max = maximo();
    Nodo *nodo = minimo_subarbol(_raiz);
    while (nodo->valor < max) {
        s << nodo->valor << ", ";
        nodo = siguiente_nodo(nodo);
    }
    s << max << "}";
}

