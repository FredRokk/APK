
#if !defined(_LOGFILE_H_)
#define _LOGFILE_H_

#include <stdexcept>
#include <string>

class LogFile
{
public:
  enum EState
  {
    es_OK,
    es_NOT_INITIALIZED,
    es_COULD_NOT_OPEN_FILE,
    es_FILENAME_IS_EMPTY
  };
  bool   write(const std::string &data);
  bool   init(const std::string &logfilename);
  EState getState() { return state_; }
  LogFile();
  explicit LogFile(const std::string &logfilename);
  ~LogFile();

private:
  std::string logfilename_;
  EState      state_;
  bool        internalWrite(const std::string &data);
};

#endif // !defined(AFX_LOGFILE_H__4A9D73D5_7588_11D3_A2E2_00104BB8D7A4__INCLUDED_)
