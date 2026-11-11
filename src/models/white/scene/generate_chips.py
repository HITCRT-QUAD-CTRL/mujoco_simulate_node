import random

def generate_dynamic_gravel_xml(x_min, x_max, y_min, y_max, count, start_index=0):
    xml_str = ""
    for i in range(count):
        # 随机位置
        x = random.uniform(x_min, x_max)
        y = random.uniform(y_min, y_max)
        # Z轴起始高度要错开，避免初始时刻就发生剧烈重叠爆炸
        # 建议让它们从上方像下雨一样掉进坑里
        z = random.uniform(0.1, 0.4) 
        
        # 随机大小
        r = random.uniform(0.015, 0.03)
        
        # 生成 XML 块
        xml_str += f"""
        <body name="stone_{start_index + i}" pos="{x} {y} {z}">
            <freejoint/>
            <geom type="sphere" size="{r}" material="stone_mat" condim="4" friction="0.8 0.02 0.01" density="2500"/>
        </body>
        """
    return xml_str

# 示例：生成 100 个石头
# 复制打印出来的内容到 MJCF 的 worldbody 中
print(generate_dynamic_gravel_xml(-0.5, 0.5, -0.5, 0.5, 100))