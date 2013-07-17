#ifndef _LUCID_UNCOPYABLE_H_
#define _LUCID_UNCOPYABLE_H_

namespace lc{

	//refer to:http://www.iclcv.org/icl-reference/ICLUtils/Uncopyable_8h_source.html

class Uncopyable{
protected:
	Uncopyable(){}
private:
	Uncopyable(const Uncopyable &other){
		(void)other;
	}
	Uncopyable &operator=(const Uncopyable &other){
		(void)other;
		return *this;
	}
};

}
#endif