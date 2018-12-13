all:
	make -C GNU\ ARM\ v7.2.1\ -\ Debug all

flash:
	commander --apack flash --serialno 000440078765 --tif swd --address 0 -d efr32bg1b232f256gm48 GNU\ ARM\ v7.2.1\ -\ Debug/clsteam.hex
	#commander --apack flash --serialno 000440078802 --tif swd --address 0 -d efr32bg1b232f256gm48 GNU\ ARM\ v7.2.1\ -\ Debug/clsteam.hex

clean:
	make -C GNU\ ARM\ v7.2.1\ -\ Debug clean
