#pragma once

#include <rqt_gui_cpp/plugin.h>
#include <rqt_line_sensor_control/ui_LineSensorControl.h>
#include "QLed.h"

#include <ros/ros.h>

#include "rqt_line_sensor_control/GetSensorsConfig.h"
#include "rqt_line_sensor_control/GetSensorsWeights.h"

#include <QWidget>
#include <QObject>
#include <QPushButton>
#include <QSignalMapper>
#include <QDoubleSpinBox>

#define NUMBER_OF_SENSORS 8

namespace linesensorcontrol {

class LineSensorControl : public rqt_gui_cpp::Plugin {
Q_OBJECT

public:

  /* ======================================================================== */
  /* Constructor/Destructor                                                   */
  /* ======================================================================== */

  LineSensorControl();

  /* ======================================================================== */
  /* Initialize/Shutdown                                                      */
  /* ======================================================================== */

  void initPlugin(qt_gui_cpp::PluginContext& context) override;

  void shutdownPlugin() override;

  /* ======================================================================== */
  /* Settings                                                                 */
  /* ======================================================================== */

  void saveSettings(qt_gui_cpp::Settings& plugin_settings,
                    qt_gui_cpp::Settings& instance_settings) const override;

  void restoreSettings(const qt_gui_cpp::Settings& plugin_settings,
                       const qt_gui_cpp::Settings& instance_settings) override;

private:

  /* ======================================================================== */
  /* Constants                                                                */
  /* ======================================================================== */

  const std::string TAG = "LineSensorControl";

  /* ======================================================================== */
  /* Variables                                                                */
  /* ======================================================================== */

  Ui::LineSensorControlWidget ui_;
  QWidget* widget_;
  
  QLed* setleds_[NUMBER_OF_SENSORS];
  QPushButton* ledButtons_[NUMBER_OF_SENSORS];

  QDoubleSpinBox* sensorWeights[NUMBER_OF_SENSORS];

  QSignalMapper* sigmap;

  ros::NodeHandle nh_;
  ros::ServiceServer config_service;
  ros::ServiceServer weights_service;
  
  bool setledsState_[NUMBER_OF_SENSORS];

  /* ======================================================================== */
  /* Methods                                                                  */
  /* ======================================================================== */



  /* ======================================================================== */
  /* Events                                                                   */
  /* ======================================================================== */



  /* ======================================================================== */
  /* Callbacks                                                                */
  /* ======================================================================== */

  bool GetSensorsConfig_callback(rqt_line_sensor_control::GetSensorsConfig::Request &req,
                                 rqt_line_sensor_control::GetSensorsConfig::Response &res);
                                  
  bool GetSensorsWeights_callback(rqt_line_sensor_control::GetSensorsWeights::Request &req,
                                    rqt_line_sensor_control::GetSensorsWeights::Response &res);
                              

protected slots:

  /* ======================================================================== */
  /* Slots                                                                    */
  /* ======================================================================== */

  void updateSetLedState(int ledNumber);


signals:

  /* ======================================================================== */
  /* Signals                                                                  */
  /* ======================================================================== */



};

} // namespace
