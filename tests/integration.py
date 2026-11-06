import json,pathlib,subprocess,tempfile,time
binary=str(pathlib.Path('dirwatch').resolve())
with tempfile.TemporaryDirectory() as tmp:
    p=subprocess.Popen([binary,tmp],stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
    try:
        assert p.stderr.readline().startswith('Watching ')
        first=pathlib.Path(tmp)/'sample".txt';first.write_text('hello');renamed=pathlib.Path(tmp)/'renamed.txt';first.rename(renamed);renamed.unlink();time.sleep(.2)
    finally:p.terminate()
    out,err=p.communicate(timeout=5);rows=[json.loads(line)for line in out.splitlines()]
    kinds={row['event']for row in rows}
    assert {'created','write-closed','moved-from','moved-to','deleted'}<=kinds,kinds
    assert any(r['name']=='sample".txt'for r in rows)
    a=next(r['cookie']for r in rows if r['event']=='moved-from');b=next(r['cookie']for r in rows if r['event']=='moved-to');assert a==b and a!=0
    assert p.returncode==0
print('Create/write/rename/delete, JSON escaping, and signal shutdown passed')
