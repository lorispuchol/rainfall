
// Decompiled from asm
class N {
private:
    char annotation[108]; // 0x6c in hexa 
    int nb;

public:
    N(int value): nb(value) {}

    void setAnnotation(char* text)
    {
      memcpy(this->annotation, text, strlen(text);
    }
    int operator+(N &right)
    {
      return this->nb + right.nb;
    }
    int operator-(N &right)
    {
      return this->nb - right.nb;
    }
};

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        exit(1);
    }

    N* obj1 = new N(5);
    N* obj2 = new N(6);

    obj1->setAnnotation(argv[1]);

    
    return 0;
}