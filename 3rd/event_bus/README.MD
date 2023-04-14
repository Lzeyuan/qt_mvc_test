# EventBus
# ���ٿ�ʼ
```c++

// �¼�����
struct HelloEvent {
  int id;
  bool sex;
};

// ���ģ�slot_Ϊshared_ptr, ���ü�����0����Ϊȡ�����ġ�
slot_ = EventBus::GetInstance().Subscribe<std::shared_ptr<HelloEvent>>(
	"10086", [this](std::shared_ptr<HelloEvent> helloEvent) {
		// Qt
		qDebug() << "HelloEvent id: " << QString::number(helloEvent->id);
});

// ����
QTimer::singleShot(1000, this, []() {
	QtConcurrent::run([]() {
		qDebug() << "Publish HelloEvent";
		auto helloEvent = std::make_shared<HelloEvent>();
		helloEvent->id = 666;
		helloEvent->sex = false;
		EventBus::GetInstance().Publish("10086", helloEvent);
	});
});
```

# ���
ģ�°�׿live-eventbus�÷�����װ[SignalSlot.h](https://github.com/chenshuo/recipes/blob/master/thread/SignalSlot.h)ʵ�֣�ʹ��cow����֤�����ߺͷ����ߵ����̰߳�ȫ�����ǲ��ܱ�֤lambda����ı����̰߳�ȫ��

# UMLͼ
![Umlͼ](images/UML.png)