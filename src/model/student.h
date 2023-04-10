#ifndef MODEL_STUDENT_H_
#define MODEL_STUDENT_H_

#include <QString>

namespace model {
struct Student {
  int id_;
  QString name_;
  bool sex;
};
}  // namespace model
#endif // MODEL_STUDENT_H_