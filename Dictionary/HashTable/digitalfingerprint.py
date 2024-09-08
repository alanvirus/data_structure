import hashlib

def hash_message(message, algorithm):
    # 根据不同的算法创建散列对象
    if algorithm == 'md5':
        hasher = hashlib.md5()
    elif algorithm == 'sha1':
        hasher = hashlib.sha1()
    elif algorithm == 'sha224':
        hasher = hashlib.sha224()
    elif algorithm == 'sha256':
        hasher = hashlib.sha256()
    elif algorithm == 'sha384':
        hasher = hashlib.sha384()
    elif algorithm == 'sha512':
        hasher = hashlib.sha512()
    else:
        raise ValueError("Unsupported algorithm")

    # 更新散列对象
    hasher.update(message.encode('utf-8'))

    # 获取十六进制格式的散列值
    hexdigest = hasher.hexdigest()

    return hexdigest

# 示例消息
message = "Hello, world1!"

# 计算不同散列算法的结果
algorithms = ['md5', 'sha1', 'sha224', 'sha256', 'sha384', 'sha512']
for algo in algorithms:
    print(f"{algo.upper()} hash of '{message}': {hash_message(message, algo)}")