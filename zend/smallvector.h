namespace Php
{

template <typename TElement, int InlineCapacity = 10>
class SmallVector
{
private:
    std::vector<TElement> _vector;
    TElement *_data;
    TElement _buffer[InlineCapacity];

public:
    SmallVector(uint32_t size) : SmallVector((int) size) {}
    SmallVector(int size)
    {
        if (size > InlineCapacity)
        {
            _vector.resize(size);
            _data = &_vector[0];
        }
        else
        {
            _data = &_buffer[0];
        }
    }

    TElement &operator[](int i)
    {
        return _data[i];
    };
};

}