#include "student_dao.h"

namespace dao {

int StudentDao::GetId() { return 11; }
QString StudentDao::GetName() { return "xiaoming"; }
bool StudentDao::GetSex() { return true; }

model::Student StudentDao::GetStudent() {
  model::Student student;
  student.id_ = 12;
  student.name_ = "limei";
  student.sex = false;
  return student;
}

}  // namespace dao