#pragma once
#include"Layer.h"
#include<vector>
class LayerManage {
public:
	LayerManage():active(nullptr){}
	void addLayer(Layer* layer);
	void deleteLayer(Layer* layer);
	void setActiveLayer(Layer* layer);
	Layer* getActiveLayer();
	QVector<Layer*>& getLayers();
private:
   QVector<Layer*> layers;
   Layer *active;







};