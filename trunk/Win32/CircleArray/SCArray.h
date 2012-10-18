/////////////////////////////////////////////////////////////////////////////////
//    SCArray for char
//    ���ߣ�zhaojietong
//    ������ѭ�����飬����
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
                char* _hp; //ͷָ��
                char* _tp; //βָ��
                char* _wp; //дָ��
                char* _rp; //��ָ��
                int _bufferSize; //��������С
                int _getLineSize; //GetLine������õ����ݴ�С
                bool _status_wp;
                bool _status_rp;
                bool _isLine(const char* data, const char* lineMask) //�жϴ�data��ʼ���Ƿ���һ�����ܽ��
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
                } //��ÿ�д������
                char* ReadBuffer()
                {
                        return _rp;
                } //��ÿɶ�������
                char* Buffer()
                {
                        return _hp;
                } //��û�����

                int GetBufferSize()
                {
                        return _bufferSize;
                } //��û�������С
                int GetReadUsableSize() //��ÿɶ��ռ��С
                {
                        CircleAdjustReadPosition();
                        return (_wp == _rp && _status_wp == _status_rp) ? 0 : ((_wp > _rp) ? (_wp - _rp): (_tp - _rp));
                }
                int GetWriteUsableSize() //��ÿ�д�ռ��С
                {
                        CircleAdjustWritePosition();
                        return (_wp == _rp && _status_wp != _status_rp) ? 0 : ((_wp >= _rp) ? (_tp - _wp): (_rp - _wp));
                }

                int GetReadPosition()
                {
                        return _rp - _hp;
                } //��ö�����λ��
                int GetWritePosition()
                {
                        return _wp - _hp;
                } //���д����λ��

                void SetReadPosition(int len) //���ö�ָ�����ƫ��
                {
                        _rp = _hp + ((_bufferSize > len) ? len : _bufferSize);
                        CircleAdjustReadPosition();
                }
                void SetWritePosition(int len) //����дָ�����ƫ��
                {
                        _wp = _hp + ((_bufferSize > len) ? len : _bufferSize);
                        CircleAdjustWritePosition();
                }
                void OffsetReadPosition(int len) //���ö�ָ���ȶ�ƫ��
                {
                        if (len < _hp - _rp)
                        {
                            len = _hp - _rp;
                        }
                        _rp = ((_tp - _rp) > len) ? (_rp + len): _tp;
                        CircleAdjustReadPosition();
                }
                void OffsetWritePosition(int len) //����дָ���ȶ�ƫ��
                {
                        if (len < _hp - _wp)
                        {
                            len = _hp - _wp;
                        }
                        _wp = ((_tp - _wp) > len) ? (_wp + len): _tp;
                        CircleAdjustWritePosition();
                }
                void CircleOffsetReadPosition(int len) //ѭ�����ö�ָ���ȶ�ƫ��
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
                void CircleOffsetWritePosition(int len) //ѭ������дָ���ȶ�ƫ��
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
                } //��ָ��ص�������ͷ
                void GotoHeadWritePosition()
                {
                        _wp = _hp;
                } //дָ��ص�������ͷ
                void CircleAdjustReadPosition() //ѭ������������λ��
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
                void CircleAdjustWritePosition() //ѭ������д����λ��
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

                void Clear() //���
                {
                        memset(_hp, '\0', _bufferSize);
                        _init();
                }

                int WriteData(const char* data, int len) //д���ݣ����سɹ�д����ֽ���
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

                int HaveData(int dataLen) //�ж��Ƿ�������
                {
                        int usableLen = GetReadUsableSize(); //�ɶ��Ŀռ��С
                        int minusLen = dataLen - usableLen; //��ռ�
                        if (dataLen < 1 || dataLen >= _bufferSize)
                        //�ռ�Ԥ�жϣ������ȡ�����ݳ��ȴ��ڻ�����
                        {
                            return 0;
                        }
                        if (usableLen == 0)
                        {
                            return 0;
                        }
                        if (minusLen > 0)
                        //�����ȡ�����ݳ���С�ڿɶ��Ŀռ��С
                        {
                                if (_rp < _wp || (_wp < _rp && minusLen > _wp - _hp))
                                {
                                    return 0;
                                }
                        }
                        return usableLen;
                }

                bool GetData(int dataLen, char* data) //��ȡ����
                {
                        int usableLen;
                        if (!(usableLen = HaveData(dataLen)))
                        //�ж��Ƿ�������ݣ��������ȡ�ɶ��Ŀռ��С
                        {
                            return false;
                        }
                        int minusLen = dataLen - usableLen; //��ռ�
                        if (minusLen > 0)
                        //�����ȡ�����ݳ���С�ڿɶ��Ŀռ��С
                        {
                                memcpy(data, _rp, usableLen); //����������ĩβ����
                                memcpy(data + usableLen, _hp, minusLen); //������������ʼ����
                                _status_rp = !_status_rp;
                                _rp = _hp + minusLen; //ƫ�ƶ�ָ��
                        }
                        else
                        {
                                memcpy(data, _rp, dataLen); //����������
                                _rp += dataLen; //ƫ�ƶ�ָ��
                        }
                        data[dataLen] = '\0';
                        return true;
                }

                bool GetLine(char* data, const int dataLen, const char* lineMask) //��һ������
                {
                        int mklen = strlen(lineMask); //�н������Ĵ�С
                        int usableLen = GetReadUsableSize(); //�ɶ��Ŀռ��С
                        int minusLen = usableLen - mklen + 1; //��ռ�
                        int i;

                        _getLineSize = 0; //��������

                        if (usableLen == 0)
                        {
                            return false;
                        }
                        if (mklen > _bufferSize)
                        //�ռ�Ԥ�жϣ������ʾ�н����Ľ��������ȴ��ڻ��������򲻶�
                        {
                            return false;
                        }
                        if (_rp == _wp && _status_rp == _status_wp)
                        //�ռ�Ԥ�жϣ���ָ���дָ�����ʱ������
                        {
                            return false;
                        }
                        if (_rp < _wp)
                        //�ռ�Ԥ�жϣ������ָ����дָ���ǰ�棬����Ҷ�дָ��֮���Ƿ����н�����
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
                                //�ռ�Ԥ�жϣ����������ĩβ�Ŀռ�С���н������Ĵ�С
                                {
                                    return false;
                                }
                                if (_rp < _wp)
                                //�ռ�Ԥ�жϣ������ָ��С��дָ��
                                {
                                    return false;
                                }
                        }

                        for (i = 0; i < minusLen && !_isLine(_rp + i, lineMask); i++)
                        //�������ݣ�ֱ�������н�����
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
                        //��������ѭ����û�������н���������ʾ��ָ�����дָ�룬���Ҷ�ָ����ڻ�������β����
                        {
                                char* lp = _hp; //�ӻ�����ͷ��ʼ��������
                                for (; lp < _wp && !_isLine(lp, lineMask); lp++)
                                //�������ݣ�ֱ�������н�����
                                {
                                        if (dataLen - 1 < _getLineSize)
                                        {
                                                _getLineSize =  - 1;
                                                return false;
                                        }
                                        data[_getLineSize++] = lp[0];
                                }
                                if (lp == _wp)
                                //��������ѭ����û�������н����������쳣
                                {
                                    return false;
                                }
                                _rp = lp + mklen; //ƫ�ƶ�ָ��
                                _status_rp = !_status_rp;
                        }
                        else
                        {
                                if (dataLen - 1 < _getLineSize)
                                {
                                        _getLineSize =  - 1;
                                        return false;
                                }
                                _rp += _getLineSize + mklen; //ƫ�ƶ�ָ��
                        }
                        data[_getLineSize] = '\0'; //���ݽ���
                        return true;
                }
                int GetLineSize()
                {
                        return _getLineSize;
                } //GetLine������õ����ݴ�С
                bool GetTurnStatus()
                {
                        return _status_rp == _status_wp;
                } //��дָ���Ƿ���ͬһ��������
        };
    }

#endif // _SCARRAY_
