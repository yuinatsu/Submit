<?xml version="1.0" encoding="UTF-8"?>
<objectList name = "Player">
	<actionNode name="LR">
		<attach name="CheckKey" state = "CntKey" key="Right">
			<attach name ="ColisionCheck"speed="5" width="24">
				<attach name="SetAnime" state = "right"/>
				<attach name="Move" x="5"/>
			</attach>	
		</attach>
		<attach name="CheckKey" state = "CntKey" key="Left">
			<attach name ="ColisionCheck"speed="-5" width="-24">
				<attach name="SetAnime" state = "left"/>
				<attach name="Move" x="-5"/>
			</attach>
		</attach>
	</actionNode>
	<actionNode name="Jump">
		<attach name ="CheckKey" key="Up">
			<attach name="Jump" time="0.65" />
		</attach>
	</actionNode>
	<actionNode name="CngColor">
		<attach name ="CheckKey" key="Btn3" >
			<attach name="CngColorOpen" flag="true"/>
		</attach>
		<attach name ="CheckKey" key="RightB" >
			<attach name="CngColorSelect" Right="48"/>
		</attach>
		<attach name ="CheckKey" key="LeftB" >
			<attach name="CngColorSelect" Left="48"/>
		</attach>
		<attach name ="CheckKey" key="Btn2" >
			<attach name="CngColorClose" flag="true"/>
		</attach>
		<attach name ="CheckKey" key="Btn1" >
			<attach name="CngColorDeside" flag="true"/>
		</attach>
	</actionNode>
	<actionNode name="Pause">
		<attach name ="CheckKey" key="Btn4">
			<attach name="Pause" flag="true" />
		</attach>
	</actionNode>
</objectList>
