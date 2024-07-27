# ʹ��stm32h750xbh6ʵ�ֵ�Linuxϵͳ

## 0. ��ĿԤ��
ʹ��stm32h750xbh6ʵ��һ��Linuxϵͳ��ʵ�ֻ�����Linux���ܣ����ļ�ϵͳ�����繦�ܡ�ͼ�ν��湦�ܵȡ���ϵͳ��ʵ�ֻ�����ָ���������GPIO���������ָ���������ͨ����SD����ָ������ļ��Ķ�ȡ��ɸ��ַ���Ĺ��ܡ�

�����ƻ�ָ����£�
- 1. GPIOָ��
1. gpio_init(port, pin, mode, speed, pull) // ��ʼ��GPIO
2. gpio_read(port, pin) // ��ȡGPIO
3. gpio_write(port, pin, value) // д��GPIO
4. gpio_toggle(port, pin) // ��תGPIO
- 2. ����ָ��
3. spi_read(name, data) // ��ȡSPI
4. spi_write(name, data) // д��SPI
5. spi_transfer(name, data) // ��дSPI
7. i2c_read(name, data) // ��ȡI2C
8. i2c_write(name, data) // д��I2C
9. i2c_transfer(name, data) // ��дI2C
11. uart_read(name, data) // ��ȡUART
12. uart_write(name, data) // д��UART
13. tim_setfreq(name, freq) // ���ö�ʱ��Ƶ��
14. tim_upcount(name) // ��ʱ������
15. tim_downcount(name) // ��ʱ������
16. tim_pwm(name, duty) // ��ʱ��PWM
17. otg_read(name, data) // ��ȡOTG
18. otg_write(name, data) // д��OTG
- 3. SD��ָ��
3. mkdir(path)
4. cd(path)
5. ls(path)
6. rm(path)
7. cp(src, dst)
8. mv(src, dst)
9. cat(path)
10. touch(path)
11. echo(str, path)
12. find(path)
- 4. LCDָ��
4. lcd_print(str)

## 1. ��Ŀ���
����Ŀ�ǻ���stm32h750xbh6ʵ�ֵ�Linuxϵͳ����Ҫ�����У�
- 1.1. ���ڵ��Թ���
- 1.2. �ļ�ϵͳ����
- 1.3. ���繦��
- 1.4. ͼ�ν��湦��
- 1.5. ����ָ���
- 1.6. SD��ָ�������
- 1.7. USB����
- 1.8. ��Դ������
- 1.9. ʱ�ӹ�����

## 2. ��Ŀ�ṹ
```
Appication // Ӧ�ó���
    |---inc // Ӧ�ó���
    `---src // �弶֧�ְ�
    
Core // �����ļ�
    |---inc // ͷ�ļ�
    `---src // Դ�ļ�
    
Drivers // �����ļ�
    |---Kernel // ΢��std��
    |---CMSIS // Cortex-M7�ں�֧��
    `---HAL // Ӳ�������
    
Middlewares // �м��
    |---FreeRTOS // ʵʱ����ϵͳ
    `---FatFS // USB OTG
FATFS // fat32�ļ�ϵͳ
    |---App // Ӧ�ó���
    `---Target // Ŀ��BSP
RAMFS // ram�ļ�ϵͳ(δ���)
    |---init.d // ��ʼ���ű�
    |---sbin // ϵͳ����
    |---usr // �û�����
    |---var // �����ļ�
    |---dev // �豸�ļ�
    |---etc // �����ļ�
    |---home // �û�Ŀ¼
    |---lib // ���ļ�
    |---mnt // ����Ŀ¼
    |---proc // ����Ŀ¼
    |---root // ��Ŀ¼
    |---tmp // ��ʱĿ¼
    |---usr // �û�Ŀ¼
    `---bin // ϵͳ����
    
Task // ����
    |---inc // ����ͷ�ļ�
    `---src // ����Դ�ļ�
Utilities // ����, JPG����
    |---JPEG // JPG����
```

## 3. ��Ŀ����
- [x] 3.1. ���ڵ��Թ���
  - [x] 3.1.1. u_print(fmt, ...)����
  - [x] 3.1.2. u_scanf(fmt, ...)����
- [x] 3.2. ͼ�ν��湦��
  - [x] 3.2.1. lcd_print(str)����
- [x] 3.3. �ļ�ϵͳ����
  - [x] 3.3.1. tlsf_malloc(size)����
  - [x] 3.3.2. tlsf_free(ptr)����
  - [x] 3.3.3. tlsf_realloc(ptr, size)����
  - [x] 3.3.5. tlsf_create()��ʼ��SDRAMΪTLSF�ڴ��
  - [x] 3.3.6. DeviceTree��Device֧��
  - [x] 3.3.7. SD����FATFS
- [ ] 3.4. ����ָ���
- [ ] 3.5. SD��ָ������� TODO
- [ ] 3.6. USB���� TODO
- [ ] 3.7. ��Դ������ TODO
- [ ] 3.8. ʱ�ӹ����� TODO

## 4. ��Ŀ����
���س����, ʹ��Clion�򿪹��̣����úù��̺󣬵�����뼴�ɡ���¼���ΪOpenocd��

## 5. �豸����
FK750M5-XBH6

## 6. ��Ŀ����
Minloha: https://blog.minloha.cn

## 7. ����TODO
- [ ] 7.1. ���SD��ָ�������
- [ ] 7.2. ���USB����
- [ ] 7.3. ��ɵ�Դ������
- [ ] 7.4. ���ʱ�ӹ�����
- [ ] 7.5. �������ָ���
- [ ] 7.6. ���ͼ�ν��湦��
- [ ] 7.8. ������繦��
- [ ] 7.9. ����ļ�ϵͳ����
- [ ] 7.10. ��ɴ��ڵ��Թ���
- [ ] 7.11. ���RAMFS�ļ�ϵͳ
- [ ] 7.12. ���BSP֧��

