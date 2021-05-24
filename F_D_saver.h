#include <list>
#include <map>

#include "stat.h"

class F_D_saver {
private:
	std::list<F_D> emptyBuf;
	std::map<F_D, Offset> F_D_buf;
	std::map<Offset, Offset> off_buf;

public:
	F_D_saver() {
		F_D_buf[0] = 1;
		F_D_buf[1] = 2;
		F_D_buf[2] = 3;
		off_buf[1] = 0;
		off_buf[2] = 1;
		off_buf[3] = 2;
	}
	~F_D_saver() {}
	void reset() {
		F_D_buf.clear();
		off_buf.clear();
		F_D_buf[0] = 1;
		F_D_buf[1] = 2;
		F_D_buf[2] = 3;
		off_buf[1] = 0;
		off_buf[2] = 1;
		off_buf[3] = 2;
	}
	F_D get() {
		if (emptyBuf.size()) {
			F_D d = emptyBuf.front();
			emptyBuf.pop_front();
			return d;
		}
		return F_D_buf.size();
	}
	Offset getOff(F_D fd) {
		if (F_D_buf.find(fd) != F_D_buf.end()) {
			Offset result = F_D_buf[fd];
			if (fd == off_buf[result])
				return result;
			else
				return 0;
		}
		return 0;
	}
	bool set(F_D fd, Offset of) {
		if (off_buf.find(of) != off_buf.end())
			return false;
		if (F_D_buf.find(fd) == F_D_buf.end()) {
			F_D_buf[fd] = of;
			off_buf[of] = fd;
			return true;
		}
		return false;
	}
	bool remove(F_D fd) {
		if (F_D_buf.find(fd) != F_D_buf.end()) {
			Offset result = F_D_buf[fd];
			if (off_buf.find(result) != off_buf.end() && off_buf[result] == fd) {
				F_D_buf.erase(fd);
				off_buf.erase(result);
				emptyBuf.push_back(fd);
				return true;
			}
		}
		return false;
	}
};
