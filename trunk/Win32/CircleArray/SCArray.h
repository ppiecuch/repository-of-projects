/////////////////////////////////////////////////////////////////////////////////
//    SCArray for char
//    作者：zhaojietong
//    描述：循环数组，声明
/////////////////////////////////////////////////////////////////////////////////
#ifndef _SCARRAY_
    #define _SCARRAY_

    #include <iostream>

    using namespace std;

    namespace SCARRAY
    {
        class SCArray
        {
            private:
                char* _hp; //头指针
                char* _tp; //尾指针
                char* _wp; //写指针
                char* _rp; //读指针
                int _bufferSize; //缓冲区大小
                int _getLineSize; //GetLine方法获得的数据大小
                bool _status_wp;
                bool _status_rp;
                bool _isLine(const char* data, const char* lineMask) //判断从data开始，是否是一个行总结符
                {
                        int mklen = strlen(lineMask);
                        for (int j = 0; j < mklen; j++)
                        {
                                if (data > _wp && (data + j) > _tp)
                                {
                                    return false;
                                }
                                if (data < _wp && (data + j) > _wp)
                                {
                                    return false;
                                }
                                if (data == _wp && _status_wp == _status_rp)
                                {
                                    return false;
                                }
                                if (data[j] != lineMask[j])
                                {
                                    return false;
                                }
                        }
                        return true;
                }
                void _init()
                {
                        _rp = _wp = _hp;
                        _tp = _hp + _bufferSize;
                        _status_wp = _status_rp = true;
                }
            public:
                SCArray(){}
                SCArray(SCArray* buf)
                {
                        _bufferSize = buf->GetBufferSize();
                        _hp = buf->Buffer();
                        _init();
                }
                SCArray(SCArray& buf)
                {
                        _bufferSize = buf.GetBufferSize();
                        _hp = buf.Buffer();
                        _init();
                }
                SCArray(char* buf, int bufSize)
                {
                        _bufferSize = bufSize;
                        _hp = buf;
                        _init();
                }
                SCArray(int bufferSize)
                {
                        _bufferSize = bufferSize;
                        _hp = new char[_bufferSize + 1];
                        memset(_hp, '\0', _bufferSize + 1);
                        _init();
                }
                ~SCArray()
                {
                        delete [] _hp;
                }

                char* WriteBuffer()
                {
                        return _wp;
                } //获得可写缓冲区
                char* ReadBuffer()
                {
                        return _rp;
                } //获得可读缓冲区
                char* Buffer()
                {
                        return _hp;
                } //获得缓冲区

                int GetBufferSize()
                {
                        return _bufferSize;
                } //获得缓冲区大小
                int GetReadUsableSize() //获得可读空间大小
                {
                        CircleAdjustReadPosition();
                        return (_wp == _rp && _status_wp == _status_rp) ? 0 : ((_wp > _rp) ? (_wp - _rp): (_tp - _rp));
                }
                int GetWriteUsableSize() //获得可写空间大小
                {
                        CircleAdjustWritePosition();
                        return (_wp == _rp && _status_wp != _status_rp) ? 0 : ((_wp >= _rp) ? (_tp - _wp): (_rp - _wp));
                }

                int GetReadPosition()
                {
                        return _rp - _hp;
                } //获得读操作位置
                int GetWritePosition()
                {
                        return _wp - _hp;
                } //获得写操作位置

                void SetReadPosition(int len) //设置读指针绝对偏移
                {
                        _rp = _hp + ((_bufferSize > len) ? len : _bufferSize);
                        CircleAdjustReadPosition();
                }
                void SetWritePosition(int len) //设置写指针绝对偏移
                {
                        _wp = _hp + ((_bufferSize > len) ? len : _bufferSize);
                        CircleAdjustWritePosition();
                }
                void OffsetReadPosition(int len) //设置读指针先对偏移
                {
                        if (len < _hp - _rp)
                        {
                            len = _hp - _rp;
                        }
                        _rp = ((_tp - _rp) > len) ? (_rp + len): _tp;
                        CircleAdjustReadPosition();
                }
                void OffsetWritePosition(int len) //设置写指针先对偏移
                {
                        if (len < _hp - _wp)
                        {
                            len = _hp - _wp;
                        }
                        _wp = ((_tp - _wp) > len) ? (_wp + len): _tp;
                        CircleAdjustWritePosition();
                }
                void CircleOffsetReadPosition(int len) //循环设置读指针先对偏移
                {
                        len = len % (_tp - _hp);
                        if (len < 0)
                        {
                                if (len < _hp - _rp)
                                {
                                        _rp = _tp - ((_hp - _rp) - len);
                                        _status_rp = !_status_rp;
                                }
                                else
                                {
                                    _rp = _rp + len;
                                }
                        }
                        else
                        {
                                if (len < _tp - _rp)
                                {
                                    _rp = _rp + len;
                                }
                                else
                                {
                                        _rp = _hp + (len - (_tp - _rp));
                                        _status_rp = !_status_rp;
                                }
                        }
                        CircleAdjustReadPosition();
                }
                void CircleOffsetWritePosition(int len) //循环设置写指针先对偏移
                {
                        len = len % (_tp - _hp);
                        if (len < 0)
                        {
                                if (len < _hp - _wp)
                                {
                                        _wp = _tp - ((_hp - _wp) - len);
                                        _status_wp = !_status_wp;
                                }
                                else
                                {
                                    _wp = _wp + len;
                                }
                        }
                        else
                        {
                                if (len < _tp - _wp)
                                {
                                    _wp = _wp + len;
                                }
                                else
                                {
                                        _wp = _hp + (len - (_tp - _wp));
                                        _status_wp = !_status_wp;
                                }
                        }
                        CircleAdjustWritePosition();
                }
                void GotoHeadReadPosition()
                {
                        _rp = _hp;
                } //读指针回到缓冲区头
                void GotoHeadWritePosition()
                {
                        _wp = _hp;
                } //写指针回到缓冲区头
                void CircleAdjustReadPosition() //循环调整读操作位置
                {
                        if (_tp == _rp && _wp < _tp && _wp > _hp)
                        {
                                _rp = _hp;
                                _status_rp = !_status_rp;
                        }
                        else if (_tp == _rp && _tp == _wp && _status_rp != _status_wp)
                        {
                                _rp = _hp;
                                _status_rp = !_status_rp;
                        }
                }
                void CircleAdjustWritePosition() //循环调整写操作位置
                {
                        if (_tp == _wp && _rp < _tp && _rp > _hp)
                        {
                                _wp = _hp;
                                _status_wp = !_status_wp;
                        }
                        else if (_tp == _rp && _tp == _wp && _status_rp == _status_wp)
                        {
                                _wp = _hp;
                                _status_wp = !_status_wp;
                        }
                }

                void Clear() //清空
                {
                        memset(_hp, '\0', _bufferSize);
                        _init();
                }

                int WriteData(const char* data, int len) //写数据，返回成功写入的字节数
                {
                        int wsize = GetWriteUsableSize();
                        if (wsize == 0 || len < 1)
                        {
                            return 0;
                        }
                        wsize = (wsize > len) ? len : wsize;
                        memcpy(_wp, data, wsize);
                        OffsetWritePosition(wsize);
                        return wsize;
                }

                int HaveData(int dataLen) //判断是否有数据
                {
                        int usableLen = GetReadUsableSize(); //可读的空间大小
                        int minusLen = dataLen - usableLen; //差空间
                        if (dataLen < 1 || dataLen >= _bufferSize)
                        //空间预判断：如果读取的数据长度大于缓冲区
                        {
                            return 0;
                        }
                        if (usableLen == 0)
                        {
                            return 0;
                        }
                        if (minusLen > 0)
                        //如果读取的数据长度小于可读的空间大小
                        {
                                if (_rp < _wp || (_wp < _rp && minusLen > _wp - _hp))
                                {
                                    return 0;
                                }
                        }
                        return usableLen;
                }

                bool GetData(int dataLen, char* data) //获取数据
                {
                        int usableLen;
                        if (!(usableLen = HaveData(dataLen)))
                        //判断是否存在数据，存在则获取可读的空间大小
                        {
                            return false;
                        }
                        int minusLen = dataLen - usableLen; //差空间
                        if (minusLen > 0)
                        //如果读取的数据长度小于可读的空间大小
                        {
                                memcpy(data, _rp, usableLen); //检索缓冲区末尾区域
                                memcpy(data + usableLen, _hp, minusLen); //检索缓冲区开始区域
                                _status_rp = !_status_rp;
                                _rp = _hp + minusLen; //偏移读指针
                        }
                        else
                        {
                                memcpy(data, _rp, dataLen); //检索缓冲区
                                _rp += dataLen; //偏移读指针
                        }
                        data[dataLen] = '\0';
                        return true;
                }

                bool GetLine(char* data, const int dataLen, const char* lineMask) //读一行数据
                {
                        int mklen = strlen(lineMask); //行结束符的大小
                        int usableLen = GetReadUsableSize(); //可读的空间大小
                        int minusLen = usableLen - mklen + 1; //差空间
                        int i;

                        _getLineSize = 0; //数据索引

                        if (usableLen == 0)
                        {
                            return false;
                        }
                        if (mklen > _bufferSize)
                        //空间预判断：如果表示行结束的结束符长度大于缓冲区，则不读
                        {
                            return false;
                        }
                        if (_rp == _wp && _status_rp == _status_wp)
                        //空间预判断：读指针和写指针相等时，不读
                        {
                            return false;
                        }
                        if (_rp < _wp)
                        //空间预判断：如果读指针在写指针的前面，则查找读写指针之间是否处在行结束符
                        {
                                for (i = 0; i < minusLen && !_isLine(_rp + i, lineMask); i++)
                                    ;
                                if (i == minusLen)
                                {
                                    return false;
                                }
                                if (_rp + i >= _wp)
                                {
                                    return false;
                                }
                        }
                        if (minusLen < 0)
                        {
                                if (_rp > _wp &&  - minusLen > _wp - _hp)
                                //空间预判断：如果缓冲区末尾的空间小于行结束符的大小
                                {
                                    return false;
                                }
                                if (_rp < _wp)
                                //空间预判断：如果读指针小于写指针
                                {
                                    return false;
                                }
                        }

                        for (i = 0; i < minusLen && !_isLine(_rp + i, lineMask); i++)
                        //检索数据，直到遇到行结束符
                        {
                                if (_rp < _wp && (_rp + i) >= _wp)
                                {
                                    return false;
                                }
                                if (_rp > _wp && (_rp + i) > _tp)
                                {
                                    return false;
                                }
                                if (_rp == _wp && _status_rp == _status_wp)
                                {
                                    return false;
                                }
                                if (dataLen - 1 < _getLineSize)
                                {
                                        _getLineSize =  - 1;
                                        return false;
                                }
                                data[_getLineSize++] = _rp[i];
                        }

                        if (i == minusLen)
                        //如果上面的循环中没有遇到行结束符，表示读指针大于写指针，并且读指针就在缓冲区结尾附近
                        {
                                char* lp = _hp; //从缓冲区头开始继续检索
                                for (; lp < _wp && !_isLine(lp, lineMask); lp++)
                                //检索数据，直到遇到行结束符
                                {
                                        if (dataLen - 1 < _getLineSize)
                                        {
                                                _getLineSize =  - 1;
                                                return false;
                                        }
                                        data[_getLineSize++] = lp[0];
                                }
                                if (lp == _wp)
                                //如果上面的循环中没有遇到行结束符，则异常
                                {
                                    return false;
                                }
                                _rp = lp + mklen; //偏移读指针
                                _status_rp = !_status_rp;
                        }
                        else
                        {
                                if (dataLen - 1 < _getLineSize)
                                {
                                        _getLineSize =  - 1;
                                        return false;
                                }
                                _rp += _getLineSize + mklen; //偏移读指针
                        }
                        data[_getLineSize] = '\0'; //数据结束
                        return true;
                }
                int GetLineSize()
                {
                        return _getLineSize;
                } //GetLine方法获得的数据大小
                bool GetTurnStatus()
                {
                        return _status_rp == _status_wp;
                } //读写指针是否在同一个周期中
        };
    }

#endif // _SCARRAY_
