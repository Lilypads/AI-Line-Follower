#pragma once

#include <rqt_gui_cpp/plugin.h>
#include <rqt_neural_network_diagnostics/ui_NeuralNetworkDiagnosics.h>

#include <ros/ros.h>
#include "geometry_msgs/Twist.h"
#include "std_msgs/Float64.h"

#include <QWidget>
#include <QObject>
#include <QTimer>

namespace neuralnetworkdiagnostics {

class NeuralNetworkDiagnostics : public rqt_gui_cpp::Plugin {
Q_OBJECT

public:

  /* ======================================================================== */
  /* Constructor/Destructor                                                   */
  /* ======================================================================== */

  NeuralNetworkDiagnostics();

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

  const std::string TAG = "NeuralNetworkDiagnostics";

  /* ======================================================================== */
  /* Variables                                                                */
  /* ======================================================================== */

  Ui::NeuralNetworkDiagnosticsWidget ui_;
  QWidget* widget_;
  QTimer* timer;

  ros::NodeHandle nh_;
  ros::Subscriber motorSubscriber;
  ros::Subscriber outputSubscriber;
  ros::Subscriber errorSubscriber;

  int valueOfMotorIndicator;
  double valueOfOutputNode;  
  double valueOfError;

  /* ======================================================================== */
  /* Methods                                                                  */
  /* ======================================================================== */



  /* ======================================================================== */
  /* Events                                                                   */
  /* ======================================================================== */



  /* ======================================================================== */
  /* Callbacks                                                                */
  /* ======================================================================== */

  void setMotorIndicatorValue(const geometry_msgs::Twist::ConstPtr& msg);
  void setOutputNodeIndicatorValue(const std_msgs::Float64::ConstPtr& msg);
  void setErrorIndicatorValue(const std_msgs::Float64::ConstPtr& msg);

protected slots:

  /* ======================================================================== */
  /* Slots                                                                    */
  /* ======================================================================== */

  void updateGuiValues();


signals:

  /* ======================================================================== */
  /* Signals                                                                  */
  /* ======================================================================== */



};

} // namespace
