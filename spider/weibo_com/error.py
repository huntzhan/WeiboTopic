
class DependencyNotInstalledError(Exception):
    def __init__(self, dep):
        self.dep = dep

    def __str__(self):
        return 'Error because lacking of dependency: %s' % self.dep


class LoginFailure(Exception):
    pass
