

def print_hex(data: bytes):  # 打印十六进制数据
    for i in data:
        print(f"0x{i:02X}", end=" ")
    print("")

def Data_Pack(cx, cy):  # 数据包函数
    if cx is None or cy is None:  # 正确判断None
        cx = 0  # 默认值为整数0
        cy = 0

    start = b'\xAA'
    end = b'\xBB'
    cx_byte = cx.to_bytes(2, byteorder='big')
    cy_byte = cy.to_bytes(2, byteorder='big')
    data = start + cx_byte + cy_byte + end
    return data

def Data_circular_ring(Color_Id):
    if Color_Id is None:    # 检查是否为 None
        Color_Id = '0'      #如果为None，则将 ID = '0',作为一个空字节

    start = b'\xCC'
    end = b'\xBB'
    ID_byte = ord(Color_Id).to_bytes(1, byteorder='big')
    data1 = start + ID_byte + end
    return data1

def Data_Color_Pack(CX, CY):  # 数据包函数
    if CX is None or CY is None:  # 正确判断None
        CX = 0  # 默认值为整数0
        CY = 0

    start = b'\xDD'
    end = b'\xBB'
    CX_byte = CX.to_bytes(2, byteorder='big')
    CY_byte = CY.to_bytes(2, byteorder='big')

    data2 = start + CX_byte + CY_byte + end

    return data2