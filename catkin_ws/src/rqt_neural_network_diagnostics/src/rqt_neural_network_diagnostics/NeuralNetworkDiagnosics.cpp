#include "rqt_neural_network_diagnostics/NeuralNetworkDiagnosics.h"

#include <pluginlib/class_list_macros.h>

namespace neuralnetworkdiagnostics {

/* ========================================================================== */
/* Constructor/Destructor                                                     */
/* ========================================================================== */

NeuralNetworkDiagnostics::NeuralNetworkDiagnostics()
    : rqt_gui_cpp::Plugin(),
      widget_(nullptr) {

  setObjectName("NeuralNetworkDiagnostics");
}

/* ========================================================================== */
/* Initialize/Shutdown                                                        */
/* ========================================================================== */

void NeuralNetworkDiagnostics::initPlugin(qt_gui_cpp::PluginContext& context) {
  widget_ = new QWidget();
  ui_.setupUi(widget_);
  if (context.serialNumber() > 1) {
    widget_->setWindowTitle(widget_->windowTitle() +
        " (" + QString::number(context.serialNumber()) + ")");
  }
  context.addWidget(widget_);
  
  //setup subscribers
  motorSubscriber = nh_.subscribe("mybot/cmd_vel", 1, &NeuralNetworkDiagnostics::setMotorIndicatorValue, this);
  outputSubscriber = nh_.subscribe("neural_net/output_node", 1, &NeuralNetworkDiagnostics::setOutputNodeIndicatorValue, this);
  errorSubscriber = nh_.subscribe("neural_net/closed_loop_error", 1, &NeuralNetworkDiagnostics::setErrorIndicatorValue, this);
  
  //setup GUI paint timer
  timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &NeuralNetworkDiagnostics::updateGuiValues);
  
  timer->start(100); //100ms
  
}

void NeuralNetworkDiagnostics::shutdownPlugin() {
  
  motorSubscriber.shutdown();  
}

/* ========================================================================== */
/* Settings                                                                   */
/* ========================================================================== */

void NeuralNetworkDiagnostics::saveSettings(
    qt_gui_cpp::Settings& plugin_settings,
    qt_gui_cpp::Settings& instance_settings) const {
}

void NeuralNetworkDiagnostics::restoreSettings(
    const qt_gui_cpp::Settings& plugin_settings,
    const qt_gui_cpp::Settings& instance_settings) {
}

/* ========================================================================== */
/* Methods                                                                    */
/* ========================================================================== */



/* ========================================================================== */
/* Events                                                                     */
/* ========================================================================== */



/* ========================================================================== */
/* Callbacks                                                                  */
/* ========================================================================== */

  void NeuralNetworkDiagnostics::setMotorIndicatorValue(const geometry_msgs::Twist::ConstPtr& msg){
    // convert the angular speed to from -1 -> +1 to 25-75 for the dial
    valueOfMotorIndicator = ((msg->angular.z) * 25.0) + 50;
  }

  void NeuralNetworkDiagnostics::setOutputNodeIndicatorValue(const std_msgs::Float64::ConstPtr& msg){
    valueOfOutputNode = msg->data;
  }
  
  void NeuralNetworkDiagnostics::setErrorIndicatorValue(const std_msgs::Float64::ConstPtr& msg){
    valueOfError = msg->data;
  }

/* ========================================================================== */
/* Slots                                                                      */
/* ========================================================================== */

  void NeuralNetworkDiagnostics::updateGuiValues(){
    
    ui_.motorDirectionPointer->setValue(valueOfMotorIndicator);
    
    //need to convert from double to text...
    //TODO make sure it works for negative numbers :)
    
    ui_.networkOutputActivation->setText(QString::number(valueOfOutputNode));
    ui_.closedLoopError->setText(QString::number(valueOfError));
  
  
  }


/* ========================================================================== */
/* Signals                                                                    */
/* ========================================================================== */



} // namespace

PLUGINLIB_EXPORT_CLASS(neuralnetworkdiagnostics::NeuralNetworkDiagnostics,
                       rqt_gui_cpp::Plugin)
