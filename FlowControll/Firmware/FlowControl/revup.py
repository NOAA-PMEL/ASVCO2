import re
import sys
from subprocess import Popen
from git import Repo, Remote

pco2_repo_dir = '../..'

def read_ver_file():
    with open('./src/version.h', 'r') as rf:
        version = None
        for f in rf:
            ver = re.search(r'v([\d.]+)', f)            
            try:
                ver = ver.group(1).split('.')
                version = {
                    'major':int(ver[0]), 
                    'minor':int(ver[1])
                    }
            except Exception as e:
                print(e)
    return version

def main(arg='minor'):
    repo = Repo('../..')

    # Check that current branch is develop
    if repo.active_branch.name != 'develop':
        print("You must first switch to 'develop' branch and merge changes")
        return

    # Read version file
    prev_ver = read_ver_file()

    if prev_ver == None:
        return 1

    # Check which to update
    if arg == "major":
        new_ver = {
            'major':prev_ver['major'] + 1,
            'minor':0
            }
    elif arg == 'minor':
        new_ver = {
            'major': prev_ver['major'],
            'minor': prev_ver['minor'] + 1
        }

    else:
        print(f"Invalid argument {arg}")
        return

    # Create new repo tag
    repo.create_tag(f"v{new_ver['major']}.{new_ver['minor']}")

    # Build the IAR project
    Popen('IarBuild.exe pCO2Sensor.ewp Debug').wait()

    # Add the changed files
    repo.git.add('-u')

    # Commit the changed files
    commit_msg= f"Rev to v{new_ver['major']}.{new_ver['minor']}"
    repo.git.commit('-m', commit_msg)

    # Push the changes to the GitHub repo
    Popen('git push origin develop').wait()

    # Exit
    print(f'Rev to {new_ver["major"]}.{new_ver["minor"]} complete')

if __name__ == "__main__":
    try:
        main(sys.argv[1])
    except Exception as e:
        print("No flag passed - minor being rev'ed")
        main()