#include "rqt_line_sensor_control/LineSensorControl.h"

#include <pluginlib/class_list_macros.h>

namespace linesensorcontrol {

/* ========================================================================== */
/* Constructor/Destructor                                                     */
/* ========================================================================== */

LineSensorControl::LineSensorControl()
    : rqt_gui_cpp::Plugin(),
      widget_(nullptr) {

  setObjectName("LineSensorControl");
}

/* ========================================================================== */
/* Initialize/Shutdown                                                        */
/* ========================================================================== */

void LineSensorControl::initPlugin(qt_gui_cpp::PluginContext& context) {
  widget_ = new QWidget();
  ui_.setupUi(widget_);
  if (context.serialNumber() > 1) {
    widget_->setWindowTitle(widget_->windowTitle() +
        " (" + QString::number(context.serialNumber()) + ")");
  }
  context.addWidget(widget_);
  
  /* Extra plugin stuff here... */
  sigmap = new QSignalMapper(this);
  
  for(int i=0; i<NUMBER_OF_SENSORS; i++){
    setleds_[i] = new QLed();
    ledButtons_[i] = new QPushButton();
    
    setleds_[i]->setState(Qt::red);
    
    ui_.setledLayout->addWidget(setleds_[i]);
    ui_.ledButtonLayout->addWidget(ledButtons_[i]);
    
    setledsState_[i] = false;
    
    connect(ledButtons_[i], SIGNAL(clicked()), sigmap, SLOT(map()));
    sigmap->setMapping(ledButtons_[i], i);

    sensorWeights[i] = new QDoubleSpinBox();
    sensorWeights[i]->setDecimals(3);
    
    ui_.sensorWeightsLayout->addWidget(sensorWeights[i]);
    
  }
  
  connect(sigmap, SIGNAL(mapped(int)), this, SLOT(updateSetLedState(int)));
  
  
  config_service = nh_.advertiseService("get_sensors_config", &LineSensorControl::GetSensorsConfig_callback, this);
  weights_service = nh_.advertiseService("get_sensors_weights", &LineSensorControl::GetSensorsWeights_callback, this);
  
}

void LineSensorControl::shutdownPlugin() {
  
  config_service.shutdown();
  weights_service.shutdown();

}

/* ========================================================================== */
/* Settings                                                                   */
/* ========================================================================== */

void LineSensorControl::saveSettings(
    qt_gui_cpp::Settings& plugin_settings,
    qt_gui_cpp::Settings& instance_settings) const {
}

void LineSensorControl::restoreSettings(
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

  bool LineSensorControl::GetSensorsConfig_callback(
                            rqt_line_sensor_control::GetSensorsConfig::Request &req,
                            rqt_line_sensor_control::GetSensorsConfig::Response &res){

    
    for(int i=0; i<NUMBER_OF_SENSORS; i++){
      res.sensorSelected.push_back(setledsState_[i]);
    }
    
    return true;
  }
                                  
  bool LineSensorControl::GetSensorsWeights_callback(
                            rqt_line_sensor_control::GetSensorsWeights::Request &req,
                            rqt_line_sensor_control::GetSensorsWeights::Response &res){
                        
    for(int i=0; i<NUMBER_OF_SENSORS; i++){
      res.sensor_weight.push_back(sensorWeights[i]->value());
    
    }
 
    return true;                              
  }
 


/* ========================================================================== */
/* Slots                                                                      */
/* ========================================================================== */

  void LineSensorControl::updateSetLedState(int ledNumber){
    setledsState_[ledNumber] = (setledsState_[ledNumber] ? false : true);
    if(setledsState_[ledNumber]){
      setleds_[ledNumber]->setState(Qt::green);
    }
    else{
      setleds_[ledNumber]->setState(Qt::red);
    }
  }

/* ========================================================================== */
/* Signals                                                                    */
/* ========================================================================== */



} // namespace

PLUGINLIB_EXPORT_CLASS(linesensorcontrol::LineSensorControl,
                       rqt_gui_cpp::Plugin)
