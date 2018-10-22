#pragma once
#include <vector>

/*! \class DelayLine
*  \brief Engine: simple delay line implemented using a ring buffer. Template - can hold various types
*  resizes without affecting read or write cursor positions
*
*/



template <class S>
class DelayLine
{

public:
    DelayLine(unsigned size) 
        : _buffer(size > 0 ? size : 1, S(0))
    {
        _size = size > 0 ? size : 1;
        _write = 0;
    }
	DelayLine()
		: _buffer(1)
	{
		_size = 1;
		_write = 0;
	}

    /// resize the delay line 
    void resize(unsigned size)
    {   
        if (size == 0) { size = 1; }
        if(size == _size){return;}

		typename std::vector<S>::size_type from;
        // handle size up or size down
        if (size > _size) {
            from = _write;
            _write = _size;
        } else {
            from = getReadIndex(size - 1);
            _write = 0;
        } 

		std::vector<S> temp = _buffer;
        _buffer.resize(size, 0.0);
        unsigned i = 0;
        while (i < size && i < _size) {
            _buffer[i++] = temp[from++ % _size];
        }
        _size = size;
    }

    /// add an input sample
    void pushSample(S sample)
    {
        _buffer[_write] = sample;
        _write = (_write + 1) % (_size);
    }

    S getDelayed(unsigned delaySamples) const
    {
        auto read = getReadIndex(delaySamples);
        return _buffer[read];
    }

    /// get the length of the delayline as unsigned int
    unsigned getSize()
    {
        return _size();
    }

    /// return the value at the write position (the position to be written)
    S getSampleAtWriteHead() const
    {
        return _buffer[_write];
    }

    /// returns vector of samples of length delaySamples, going delaySamples back from the most recently written 
    std::vector<S> getHistory(unsigned delaySamples) const
    {
        std::vector<S> out(delaySamples);
        if (delaySamples == 0) { return out; }
        auto read = getReadIndex( delaySamples-1);
        auto end = _write;
        if(end <= read){end += _size;}

        while (read < end) {
            out[--delaySamples] = _buffer[read % _size];
            ++read;
        }
        return out;
    }

    /// set whole delay line to 0
    void clear() 
    {
        for(auto &sample : _buffer){
            sample = 0;
        }
    }

    /// returns size of the delayline in the native size_type
    typename std::vector<S>::size_type size() const
    {
        return _buffer.size();
    }

private:
    std::vector<S> _buffer;
    unsigned _size;
    typename std::vector<S>::size_type _write;

    typename std::vector<S>::size_type getReadIndex(unsigned delayTime) const
    {
        if (delayTime >= _size) { 
            throw std::invalid_argument("cannot set delay longer than line length"); 
        }
        int read = (int)_write - ((int)delayTime+1);
        if(read<0) {read += _size;}
        return read;
    }

};