#ifndef DAO_STUDENT_H_
#define DAO_STUDENT_H_

#include <QString>
#include "src/model/student.h"

namespace dao {
class StudentDao {
 public:
  StudentDao() = default;
  int GetId();
  QString GetName();
  bool GetSex();
  model::Student GetStudent();
};
}  // namespace dao
#endif  // DAO_STUDENT_H_