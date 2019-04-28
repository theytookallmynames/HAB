#ifndef HAB_LOGGING_H
#define HAB_LOGGING_H

namespace HAB {
namespace Logging {

bool init();
void logMissionData(String data);
void logSystemData(String data);

} // namespace Logging
} // namespace HAB

#endif // HAB_LOGGING_H
